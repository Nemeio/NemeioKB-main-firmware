/*
 * SystemRuntime.cpp
 *
 *  Created on: 14 sept. 2021
 *      Author: thomas
 */

#include <utility>
#include <cstring>
#include "FileSystemAssembly.hpp"
#include "AssemblyHardware.h"
#include "SystemRuntime.hpp"
#include "etl/string.h"
#include "FirmwareUpdateTimeouts.hpp"
#include "BleDfuCommitVersionCmd.hpp"
#ifdef DEVICE_PROPERTIES_DEBUG_STORAGE
#include "DevicePropertiesEnvStorage.hpp"
#else
#include "DevicePropertiesBlockDeviceStorage.hpp"
#endif
#include "DevicePropertiesRamStorage.hpp"
#include "usb_device.h"
#include "FileSystemVersion.hpp"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "runtime"

extern const uint8_t update_package_ecdsa_public_key[64];

SystemRuntime::SystemRuntime()
    : mAssembly(mHardwareAssembly, mFileSystemAssembly)
    , mFlashInterface(mHardwareAssembly.getWatchdogItem())
    , mSfu(mFlashInterface)
    , mDisplayer(mStreamDataReadTask)
    , mFirmwareEnvironmentRedundant(mHardwareAssembly.getCRC32(),
                                    mFileSystemAssembly.getPartitionFileSystem(
                                        FileSystemPartitionLabel::FIRMWARE_ENV_BANK_A),
                                    rawfs::RAWFS_ROOT_PATH,
                                    mFileSystemAssembly.getPartitionFileSystem(
                                        FileSystemPartitionLabel::FIRMWARE_ENV_BANK_B),
                                    rawfs::RAWFS_ROOT_PATH)
    , mSha256(mHardwareAssembly.getCRC32())
    , mEdcsaP256(etl::array_view<const std::byte>{reinterpret_cast<const std::byte*>(
                                                      update_package_ecdsa_public_key),
                                                  sizeof(update_package_ecdsa_public_key)},
                 mHardwareAssembly.getCRC32())
    , mSignatureVerifier(mSha256, mEdcsaP256)
    , mUsbSerial(mAppCommProxy)
    , mBleSerial(mAppCommProxy, mHardwareAssembly.getBleItem())
    , mAppCommProxy(mUsbSerial, mBleSerial, mHardwareAssembly.getCRC32())
    , mEventLogAdapter(mFileSystemAssembly.getPartitionFileSystem(EVENT_LOG_PART),
                       EVENT_LOG_FILE_NAME)
    , mLogger(mEventLogAdapter)
{
}


void SystemRuntime::shutdown()
{
    /* Turn off external devices before shutdown */
    mHardwareAssembly.getBleItem().powerOff();
    mHardwareAssembly.getScreenItem().getAssociatedScreenController().setPower(false);

    // Shutdown
    mHardwareAssembly.getPowerController().shutdown();
}


void SystemRuntime::start()
{
#ifdef ROLLBACK_TEST
    while(true)
        ;
#endif // ROLLBACK_TEST
    mHardwareAssembly.getBleItem().add_observer(*this);

    mHardwareAssembly.init_Assembly();
    mFileSystemAssembly.init_Assembly(mHardwareAssembly.getExternalFlash(),
                                      mHardwareAssembly.getExternalSecureFlashIO(),
                                      mHardwareAssembly.getITEFlash(),
                                      mFlashInterface);
    mAssembly.init_Assembly();

    mHardwareAssembly.start_Assembly();
    mAssembly.start_Assembly();

#ifdef APP_GOLDEN_IMAGE
    mSfu.recover();
    return; /* Force optimization */
#endif

    if(!mFirmwareEnvironmentRedundant.init())
    {
        LOG_WARN(MODULE, "Invalid env, format the flash");
        format();

        ASSERT(mFirmwareEnvironmentRedundant.reset());
    }
    else
    {
        mHardwareAssembly.getWatchdogItem().refresh();
    }

    FirmwareEnvironment env;
    ASSERT(mFirmwareEnvironmentRedundant.getActiveEnvironment(env));

    if(env.getFileSystemVersion() != FILE_SYSTEM_VERSION)
    {
        LOG_WARN(MODULE, "Invalid fs version , format the app fs");
        formatApplication();

        env.setFileSystemVersion(FILE_SYSTEM_VERSION);
        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));
    }

    /* Start FS assembly after formatting */
    mFileSystemAssembly.start_Assembly();

    mAssembly.initPostFS_Assembly();

#ifdef DEVICE_PROPERTIES_DEBUG_STORAGE
    DevicePropertiesEnvStorage devicePropertiesStorage(env);
#else
    DevicePropertiesBlockDeviceStorage devicePropertiesStorage(
        mFileSystemAssembly.getPartitionBlockDevice(
            FileSystemPartitionLabel::INTERNAL_OTP_PARTITION));
#endif
    DeviceProperties deviceProperties(devicePropertiesStorage);

    configureBle(deviceProperties);

    mDisplayer.init(mHardwareAssembly.getScreenItem());

    waitBleStarted();
    if(!checkRecoveryUpdate(env))
    {
        validateUpdate(env);
    }

    mHardwareAssembly.getBleItem().configure();
    waitBleReady();

    FirmwareUpdateBank updatePart = getUpdateBank(env);
    FirmwareUpdateBank rollbackPart = getRollbackBank(env);

    mFirmwareUpdateAssembly.emplace(mFlashInterface,
                                    mSfu,
                                    mHardwareAssembly,
                                    mStreamDataReadTask,
                                    mDecompressor,
                                    /* Update partition */
                                    mFileSystemAssembly.getPartitionFileSystem(
                                        FirmwareUpdateBankToFileSystemPartition(updatePart)),
                                    rawfs::RAWFS_ROOT_PATH,
                                    /* Rollback partition */
                                    mFileSystemAssembly.getPartitionFileSystem(
                                        FirmwareUpdateBankToFileSystemPartition(rollbackPart)),
                                    rawfs::RAWFS_ROOT_PATH,
                                    /* ITE partition */
                                    mFileSystemAssembly.getPartitionBlockDevice(
                                        FileSystemPartitionLabel::ITE_PARTITION),
                                    mSignatureVerifier,
                                    mSharedBufferContainer);

    IGPIOController& gpioController = mHardwareAssembly.getStm32GPIOController();
    configureGpios(gpioController);

#ifdef CEM_TESTS
    if(gpioController.read(CEM_TEST_GPIO_PIN) == CEM_TEST_GPIO_PIN_EXPECTED_STATE)
    {
        initUsb(SerialNumber{"000000000CEM"});
        startCemTests();
    }
    else
#endif // CEM_TESTS
#ifndef DEV_BYPASS_TESTBENCH
        if(!env.hasPassedElectricalTests()
           || gpioController.read(ELECTRICAL_TEST_GPIO_PIN)
                  == ELECTRICAL_TEST_GPIO_PIN_EXPECTED_STATE
           || !deviceProperties.getTestBenchId().isValid())
    {
        initUsb(SerialNumber{"000000TBELEC"});
        startElecTests(env, deviceProperties);

        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));
    }
    else if(env.hasPassedElectricalTests()
            && (!env.hasPassedFunctionalTests() || !deviceProperties.isProvisioned()))
    {
        initUsb(SerialNumber{"000000TBFUNC"});
        startFunctionalTests(env, deviceProperties);

        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));
    }
    else
#endif // DEV_BYPASS_TESTBENCH
        if(mRollback)
    {
        /* Reset update bank since update has failed */
        resetUpdateBank(updatePart);

        SystemUpdate& systemRollback = mFirmwareUpdateAssembly.value().getSystemRollbackItem();

        systemRollback.registerListener(mAssembly.getLedInformationItem());
        systemRollback.registerComponentUpdateListener(*this);
        systemRollback.registerListener(*this);

        initUsb(deviceProperties.getSerialNumber());
        mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(
            fwupdatetimeouts::ROLLBACK_WATCHDOG_TIMEOUT_SECONDS);
        systemRollback.runUpdate();

        env.setPendingUpdate(false);
        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));

        mHardwareAssembly.getSystemItem().reset();
    }
    else
    {
        SystemUpdate& systemUpdate = mFirmwareUpdateAssembly.value().getSystemUpdateItem();
        systemUpdate.registerListener(mAssembly.getLedInformationItem());
        systemUpdate.registerListener(*this);

        initUsb(deviceProperties.getSerialNumber());

        LOG_INFO(MODULE, "Serial number: " << deviceProperties.getSerialNumber().getString());
        LOG_INFO(MODULE, "BLE MAC: " << deviceProperties.getBleMacAddress());
        startApplication(deviceProperties);
    }

    mHardwareAssembly.getWatchdogItem().refresh();

    shutdown();
}

void SystemRuntime::initUsb(const SerialNumber& serialNumber)
{
    MX_USB_DEVICE_Init(serialNumber.getString().data());

    mUsbSerial.init();
    mAppCommProxy.init();
}

FileSystemPartitionLabel SystemRuntime::FirmwareUpdateBankToFileSystemPartition(
    FirmwareUpdateBank bank)
{
    FileSystemPartitionLabel part = FileSystemPartitionLabel::UNKNOWN;
    switch(bank)
    {
    case FirmwareUpdateBank::BANK_A:
        part = FileSystemPartitionLabel::FIRMWARE_UPDATE_BANK_A;
        break;
    case FirmwareUpdateBank::BANK_B:
        part = FileSystemPartitionLabel::FIRMWARE_UPDATE_BANK_B;
        break;
    default:
        break;
    }

    return part;
}

FirmwareUpdateBank SystemRuntime::getUpdateBank(FirmwareEnvironment& env)
{
    return env.getNextFirmwareUpdateBank();
}

FirmwareUpdateBank SystemRuntime::getRollbackBank(FirmwareEnvironment& env)
{
    FirmwareUpdateBank part = FirmwareUpdateBank::UNKNOWN;
    switch(env.getNextFirmwareUpdateBank())
    {
    case FirmwareUpdateBank::BANK_A:
        part = FirmwareUpdateBank::BANK_B;
        break;
    case FirmwareUpdateBank::BANK_B:
        part = FirmwareUpdateBank::BANK_A;
        break;
    default:
        break;
    }

    return part;
}

void SystemRuntime::configureGpios(IGPIOController& gpioController)
{
    GPIOInConfig gpioInConfig;

    gpioInConfig.mode = IGPIO_INMODE_IN;
    gpioInConfig.pull = IGPIO_PULL_UP;
    gpioInConfig.itMode = IGPIO_ITMODE_UNDEFINED;
    gpioController.configureInput(ELECTRICAL_TEST_GPIO_PIN, gpioInConfig);

    GPIOOutConfig gpioOutConfig;
    gpioOutConfig.outType = IGPIO_OUTTYPE_OPENDRAIN;
    gpioOutConfig.pull = IGPIO_NO_PULL;
    gpioController.configureOutput(Electrical_Test_Passed, gpioOutConfig);

    gpioController.clear(Electrical_Test_Passed);
}

benchmark::TestBenchStatus SystemRuntime::startElecTests(FirmwareEnvironment& env,
                                                         DeviceProperties& deviceProperties)
{
    const SystemUpdateFile& systemRollbackFile = mFirmwareUpdateAssembly.value()
                                                     .getSystemRollbackFile();

    VerifiedPackageFileSystem packageFileSystem(systemRollbackFile.getFileSystem(),
                                                systemRollbackFile.getFilePath(),
                                                mSignatureVerifier,
                                                mSharedBufferContainer);
    packageFileSystem.mount();

    TestBenchId testBenchId = deviceProperties.getTestBenchId();

    mRuntime.emplace<benchmark::ElectricalTests>(mHardwareAssembly,
                                                 mAppCommProxy,
                                                 mLogger,
                                                 mFileSystemAssembly.getPartitionFileSystem(
                                                     EVENT_LOG_PART),
                                                 EVENT_LOG_FILE_NAME,
                                                 packageFileSystem,
                                                 testBenchId);
    benchmark::TestBenchStatus ret = etl::get<benchmark::ElectricalTests>(mRuntime).run();

    if(ret != benchmark::TestBenchStatus::IGNORE)
    {
        bool validated = (ret == benchmark::TestBenchStatus::VALIDATE);

        if(validated)
        {
            deviceProperties.setTestBenchId(testBenchId);
        }

        env.passedElectricalTests(validated);
        env.passedFunctionalTests(false); /* Next reboot, start functional testing */
    }

    return ret;
}

void SystemRuntime::startFunctionalTests(FirmwareEnvironment& env,
                                         DeviceProperties& deviceProperties)
{
    const SystemUpdateFile& systemRollbackFile = mFirmwareUpdateAssembly.value()
                                                     .getSystemRollbackFile();

    VerifiedPackageFileSystem packageFileSystem(systemRollbackFile.getFileSystem(),
                                                systemRollbackFile.getFilePath(),
                                                mSignatureVerifier,
                                                mSharedBufferContainer);
    packageFileSystem.mount();

    DevicePropertiesRamStorage ramStorage;
    DeviceProperties tempDeviceProperties(ramStorage);
    tempDeviceProperties.setTestBenchId(deviceProperties.getTestBenchId());
    tempDeviceProperties.setProvisioningData(deviceProperties.getSerialNumber(),
                                             deviceProperties.getBleMacAddress());

    mRuntime.emplace<benchmark::FunctionalTests>(mHardwareAssembly,
                                                 mAppCommProxy,
                                                 mLogger,
                                                 mFileSystemAssembly.getPartitionFileSystem(
                                                     EVENT_LOG_PART),
                                                 EVENT_LOG_FILE_NAME,
                                                 packageFileSystem,
                                                 tempDeviceProperties);
    benchmark::TestBenchStatus ret = etl::get<benchmark::FunctionalTests>(mRuntime).run();

    if(ret != benchmark::TestBenchStatus::IGNORE)
    {
        bool validated = (ret == benchmark::TestBenchStatus::VALIDATE);

        if(validated)
        {
            deviceProperties.setProvisioningData(tempDeviceProperties.getSerialNumber(),
                                                 tempDeviceProperties.getBleMacAddress());
        }
        env.passedFunctionalTests(validated);
    }
}

#ifdef CEM_TESTS
void SystemRuntime::startCemTests()
{
    mRuntime.emplace<benchmark::CemTests>(mHardwareAssembly);
    etl::get<benchmark::CemTests>(mRuntime).run();
}
#endif

void SystemRuntime::startApplication(DeviceProperties& deviceProperties)
{
    mRuntime.emplace<Application>(mHardwareAssembly,
                                  mFileSystemAssembly,
                                  mHardwareAssembly.getScreenItem(),
                                  mAssembly,
                                  mDisplayer,
                                  mFirmwareUpdateAssembly.value().getSystemUpdateItem(),
                                  deviceProperties,
                                  mDecompressor,
                                  mLogger,
                                  mAppCommProxy,
                                  mSharedBufferContainer);
    mHardwareAssembly.getWatchdogItem().refresh();
    etl::get<Application>(mRuntime).run();
}

void SystemRuntime::onUpdateEvent(UpdateStatus status)
{
    if(mRollback)
    {
        mHardwareAssembly.getWatchdogItem().refresh();
    }

    if(status == UpdateStatus::START)
    {
        mDisplayer.updateStart();
        mHardwareAssembly.initITEFlash();
    }
    else if(status == UpdateStatus::END)
    {
        mHardwareAssembly.deinitITEFlash();
        mDisplayer.updateStop();
    }

    if(!mRollback && (status == UpdateStatus::START))
    {
        FirmwareEnvironment env;
        ASSERT(mFirmwareEnvironmentRedundant.getActiveEnvironment(env));
        env.setPendingUpdate(true);
        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));
    }
}

void SystemRuntime::notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus)
{
    if(!mRollback)
    {
        return;
    }

    mHardwareAssembly.getWatchdogItem().refresh();

    /* Use Update class to rollback, we need to update notification type */
    if(updateStatus.mType == UpdateStatusDefs::UpdateStatusNotificationType::UPDATE_PROGRESS)
    {
        updateStatus.mType = UpdateStatusDefs::UpdateStatusNotificationType::ROLLBACK_PROGRESS;
    }

    UpdateStatusCmd status = UpdateStatusCmd(mAppCommProxy);
    status.notifyUpdateStatus(updateStatus);
}

void SystemRuntime::resetUpdateBank(FirmwareUpdateBank bank)
{
    const uint32_t wdgTimeout = mHardwareAssembly.getWatchdogItem().getTimeoutSeconds();
    mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(FORMAT_WATCHDOG_TIMEOUT_SECONDS);
    PackageFileSystem(mFileSystemAssembly.getPartitionFileSystem(
                          FirmwareUpdateBankToFileSystemPartition(bank)),
                      rawfs::RAWFS_ROOT_PATH)
        .format();
    /* Restore watchdog timeout */
    mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(wdgTimeout);
}

bool SystemRuntime::checkRecoveryUpdate(FirmwareEnvironment& env)
{
    bool ret = false;

    if(mSfu.isRecoveryUpdatePerformed())
    {
        // Reset both banks after an update from recovery mode

        // If image validation has failed, reset immediately the system without resetting banks
        // In this case, the bootloader will perform a rollback
        // so banks will still be up-to-date
        if(!mSfu.validateFirmware())
        {
            mHardwareAssembly.getSystemItem().reset();
        }

        LOG_WARN(MODULE, "Recovery detected, reset the banks");

        FirmwareUpdateBank updatePart = FirmwareUpdateBank::BANK_A;
        FirmwareUpdateBank rollbackPart = FirmwareUpdateBank::BANK_B;

        env.setNextFirmwareUpdateBank(updatePart);
        env.setPendingUpdate(false);
        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));

        resetUpdateBank(updatePart);
        resetUpdateBank(rollbackPart);

        mSfu.resetBackupInfo();

        ret = true;
    }

    return ret;
}

void SystemRuntime::validateUpdate(FirmwareEnvironment& env)
{
    /* Switch next update bank without checking version */
    FirmwareUpdateBank updatePart = getUpdateBank(env);
    FirmwareUpdateBank rollbackPart = getRollbackBank(env);

    if(env.isPendingUpdate() && updatePart != FirmwareUpdateBank::UNKNOWN)
    {
        mHardwareAssembly.getWatchdogItem().refresh();
        /*  Validate firmware or rollback if necessary */
        packageVersion expectedNrfVersion;

        mRollback = !UpdateEngine::isUpdateValid(mHardwareAssembly,
                                                 mFileSystemAssembly.getPartitionFileSystem(
                                                     FirmwareUpdateBankToFileSystemPartition(
                                                         updatePart)),
                                                 rawfs::RAWFS_ROOT_PATH,
                                                 expectedNrfVersion,
                                                 mLogger,
                                                 mSignatureVerifier,
                                                 mSharedBufferContainer);

        if(!mRollback)
        {
            /* Validate firmwares */
            mHardwareAssembly.getWatchdogItem().refresh();

            mBleStartedSignal.clear();
            BleDfuCommitVersionCmd commitVersionCmd;
            commitVersionCmd.setVersion(expectedNrfVersion.major, expectedNrfVersion.minor);

            mHardwareAssembly.getBleItem().sendCommand(
                BleApplicationTxCommandRef(etl::reference_wrapper(commitVersionCmd)));
            bool dfuCommitSuccess = waitBleStarted();

            mRollback = !dfuCommitSuccess;

            if(dfuCommitSuccess)
            {
                bool sfuValidateSuccess = mSfu.validateFirmware();
                mRollback = !sfuValidateSuccess;
            }
        }

        if(!mRollback)
        {
            /* Rollback partition becomes the next bank to store firmware */
            env.setNextFirmwareUpdateBank(rollbackPart);
            env.setPendingUpdate(false);
            ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));

            /* Reset old bank since update has suceeded */
            mHardwareAssembly.getWatchdogItem().refresh();
            resetUpdateBank(rollbackPart);
        }
        mHardwareAssembly.getWatchdogItem().refresh();
    }
    else if(FirmwareUpdateBank::UNKNOWN == updatePart)
    {
        updatePart = FirmwareUpdateBank::BANK_A;
        rollbackPart = FirmwareUpdateBank::BANK_B;

        env.setNextFirmwareUpdateBank(updatePart);
        env.setPendingUpdate(false);
        ASSERT(mFirmwareEnvironmentRedundant.writeEnvironment(env));

        resetUpdateBank(updatePart);
        resetUpdateBank(rollbackPart);
    }
}

void SystemRuntime::configureBle(DeviceProperties& deviceProperties)
{
    etl::string<BLEDefs::DEVICE_NAME_MAX_SIZE> deviceName;
    deviceName.assign("Nemeio_");

    if(deviceProperties.isProvisioned())
    {
        mHardwareAssembly.getBleItem().setDeviceMacAddress(deviceProperties.getBleMacAddress());

        deviceName.append(deviceProperties.getSerialNumber().getString().data());
    }
    else
    {
        deviceName.append("Keyboard");
    }

    /* Send Device name */
    mHardwareAssembly.getBleItem().setDeviceName(deviceName);

    /* Send protocol version to NRF */
    BLEDefs::bleDeviceProperties properties = {0};
    snprintf(properties.software_revision,
             sizeof(properties.software_revision),
             "%d.%d",
             HARDWARE_REVISION,
             PROTOCOL_VERSION);

    mHardwareAssembly.getBleItem().setDeviceProperties(properties);
}

bool SystemRuntime::waitBleStarted()
{
    bool ret = mBleStartedSignal.wait(DelayMs(NRF_WAIT_TIMEOUT_MS));
    mHardwareAssembly.getWatchdogItem().refresh();

    if(!ret)
    {
        LOG_ERR(MODULE,
                "BLE not started "
                    << TechnicalErrorDefs::TechnicalError(Identification::ModuleId::BLE,
                                                          static_cast<uint16_t>(
                                                              BLEDefs::ErrCode::DEVICE_NOT_READY)));
    }
    return ret;
}

bool SystemRuntime::waitBleReady()
{
    bool ret = mBleReadySignal.wait(DelayMs(NRF_WAIT_TIMEOUT_MS));
    mHardwareAssembly.getWatchdogItem().refresh();

    if(!ret)
    {
        LOG_ERR(MODULE,
                "BLE not ready " << TechnicalErrorDefs::TechnicalError(
                    Identification::ModuleId::BLE,
                    static_cast<uint16_t>(BLEDefs::ErrCode::DEVICE_NOT_READY)));
    }
    else
    {
        LOG_INFO(MODULE, "BLE ready");
    }
    return ret;
}

void SystemRuntime::notification(BleEvent event)
{
    switch(event)
    {
    case BleEvent::STARTED:
        mBleStartedSignal.set();
        break;
    case BleEvent::READY:
        mBleReadySignal.set();
        break;
    default:
        break;
    }
}

void SystemRuntime::format()
{
    const uint32_t wdgTimeout = mHardwareAssembly.getWatchdogItem().getTimeoutSeconds();
    mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(FORMAT_WATCHDOG_TIMEOUT_SECONDS);
    mFileSystemAssembly.format();
    /* Restore watchdog timeout */
    mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(wdgTimeout);
}

void SystemRuntime::formatApplication()
{
    const uint32_t wdgTimeout = mHardwareAssembly.getWatchdogItem().getTimeoutSeconds();
    mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(FORMAT_WATCHDOG_TIMEOUT_SECONDS);
    mFileSystemAssembly.formatApplication();
    /* Restore watchdog timeout */
    mHardwareAssembly.getWatchdogItem().setTimeoutSeconds(wdgTimeout);
}
