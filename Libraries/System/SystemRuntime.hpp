/*
 * SystemRuntime.hpp
 *
 *  Created on: 14 sept. 2021
 *      Author: thomas
 */

#ifndef SYSTEM_SYSTEMRUNTIME_HPP_
#define SYSTEM_SYSTEMRUNTIME_HPP_

#include "etl/variant.h"
#include <optional>
#include "ElectricalTests.hpp"
#include "FunctionalTests.hpp"
#ifdef CEM_TESTS
#include "CemTests.hpp"
using CemTestsInVariant = benchmark::CemTests;
#else
using CemTestsInVariant = std::monostate;
#endif
#include "Application.hpp"
#include "FirmwareEnvironmentRedundant.hpp"
#include "FlashInterface.hpp"
#include "SFU.hpp"
#include "BleEventObserver.hpp"
#include "Signal.hpp"
#include "FileLogAdapter.hpp"
#include "Logger.hpp"
#include "Sha256.hpp"
#include "EcdsaP256.hpp"
#include "EcdsaSignatureVerifier.hpp"
#include "SharedBufferContainer.hpp"

class SystemRuntime : public IUpdateListener, public IUpdateStatusListener, public BleEventObserver
{
public:
    SystemRuntime();
    ~SystemRuntime() final = default;

    void start();

private:
    static constexpr uint32_t FORMAT_WATCHDOG_TIMEOUT_SECONDS = 30;
    static constexpr uint32_t WAIT_FOR_SHUTDOWN_TIMEOUT_MS = 100;

    static constexpr GPIOPin_t ELECTRICAL_TEST_GPIO_PIN = Electrical_Test_Enter;
    static constexpr GPIOPinState ELECTRICAL_TEST_GPIO_PIN_EXPECTED_STATE = IGPIO_STATE_LOW;

    static constexpr GPIOPin_t CEM_TEST_GPIO_PIN = Key58_Shift_Left;
    static constexpr GPIOPinState CEM_TEST_GPIO_PIN_EXPECTED_STATE = IGPIO_STATE_LOW; /* pressed */

    static constexpr uint32_t NRF_WAIT_TIMEOUT_MS = 2000;
    static_assert(NRF_WAIT_TIMEOUT_MS < (WATCHDOG_TIMEOUT_SECONDS * NB_MILLISECS_IN_SECS));

    SharedBufferContainer mSharedBufferContainer;

    AssemblyHardware mHardwareAssembly;
    FileSystemAssembly mFileSystemAssembly;
    AssemblyHAL mAssembly;

    /* Needed by firmware update and update validation */
    FlashInterface mFlashInterface;
    SFU mSfu;

    /* Needed by application and firmware update */
    TinyDeflateDecompressor mDecompressor;
    StreamDataReadTask mStreamDataReadTask;

    Displayer mDisplayer;

    FirmwareEnvironmentRedundant mFirmwareEnvironmentRedundant;

    Sha256 mSha256;
    EcdsaP256 mEdcsaP256;
    EcdsaSignatureVerifier mSignatureVerifier;
    std::optional<FirmwareUpdateAssembly> mFirmwareUpdateAssembly;

    etl::variant<etl::monostate,
                 benchmark::FunctionalTests,
                 benchmark::ElectricalTests,
                 CemTestsInVariant,
                 Application>
        mRuntime;

    /* Used for Rollback notification an application */
    USBSerial mUsbSerial;
    BleSerial mBleSerial;
    AppCommProxy mAppCommProxy;

    /* Used for rollback */
    bool mRollback = false;

    Signal mBleStartedSignal;
    Signal mBleReadySignal;

    FileLogAdapter<LOG_BUFFER_SIZE> mEventLogAdapter;
    Logger mLogger;

    void configureGpios(IGPIOController& gpioController);

    benchmark::TestBenchStatus startElecTests(FirmwareEnvironment& env,
                                              DeviceProperties& deviceProperties);
    void startFunctionalTests(FirmwareEnvironment& env, DeviceProperties& deviceProperties);
#ifdef CEM_TESTS
    void startCemTests();
#endif
    void startApplication(DeviceProperties& deviceProperties);

    FileSystemPartitionLabel FirmwareUpdateBankToFileSystemPartition(FirmwareUpdateBank bank);
    FirmwareUpdateBank getUpdateBank(FirmwareEnvironment& env);
    FirmwareUpdateBank getRollbackBank(FirmwareEnvironment& env);

    void initUsb(const SerialNumber& serialNumber);

    void onUpdateEvent(UpdateStatus status) final;
    void notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus) final;

    bool checkRecoveryUpdate(FirmwareEnvironment& env);
    void validateUpdate(FirmwareEnvironment& env);

    void resetUpdateBank(FirmwareUpdateBank bank);

    bool waitBleStarted();
    bool waitBleReady();

    void configureBle(DeviceProperties& deviceProperties);

    void notification(BleEvent event) final;

    void shutdown();

    void format();
    void formatApplication();
};

#endif /* SYSTEM_SYSTEMRUNTIME_HPP_ */
