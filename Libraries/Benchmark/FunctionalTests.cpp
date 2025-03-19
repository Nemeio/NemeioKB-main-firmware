/*
 * FunctionalTests.cpp
 *
 *  Created on: 1 juin 2021
 *      Author: thomas
 */

#include "FunctionalTests.hpp"

using namespace benchmark;

FunctionalTests::FunctionalTests(AssemblyHardware& hardwareAssembly,
                                 AppCommProxy& appComm,
                                 Logger& logger,
                                 fs::IFileSystem& eventLogFileSystem,
                                 std::string_view eventLogFilePath,
                                 PackageFileSystem& firmwarePackageFileSystem,
                                 DeviceProperties& deviceProperties)
    : m_hardwareAssembly(hardwareAssembly)
    , m_logDataTransport(logger)
    , m_firmwarePackageFileSystem(firmwarePackageFileSystem)
    , m_log(m_logDataTransport)
    , mSetLedCmd(appComm, m_hardwareAssembly)
    , mSetFrontlightPowerCmd(appComm, m_hardwareAssembly)
    , mCheckComponentsCmd(appComm, m_hardwareAssembly, m_log, m_firmwarePackageFileSystem)
    , mClearScreenCmd(appComm, m_hardwareAssembly.getScreenItem())
    , mSetAdvertisingCmd(appComm, m_hardwareAssembly.getBleItem(), deviceProperties)
    , mGetTestBenchIdCmd(appComm, deviceProperties.getTestBenchId())
    , mDevicePropertiesProvisioningCmd(appComm, deviceProperties)
    , mCheckIfBatteryPresentCmd(appComm, m_hardwareAssembly)
    , mExitFunctionalTestsCmd(appComm)
    , mTestBenchKeypad(m_hardwareAssembly)
    , mGetPressedKeysCmd(appComm, mTestBenchKeypad)
    , mCheckerBoard(hardwareAssembly)
    , mDisplayCheckerBoardCmd(appComm, mCheckerBoard)
    , mReceiveDataCmd(appComm)
    , mReceiveEventLogCmd(eventLogFileSystem, eventLogFilePath)
    , mVersionsCmd(appComm,
                   m_hardwareAssembly.getBleItem(),
                   m_hardwareAssembly.getScreenItem().getAssociatedScreenController())
{
    appComm.registerRxListener(mSetLedCmd);
    appComm.registerRxListener(mSetFrontlightPowerCmd);
    appComm.registerRxListener(mCheckComponentsCmd);
    appComm.registerRxListener(mClearScreenCmd);
    appComm.registerRxListener(mSetAdvertisingCmd);
    appComm.registerRxListener(mGetTestBenchIdCmd);
    appComm.registerRxListener(mDevicePropertiesProvisioningCmd);
    appComm.registerRxListener(mCheckIfBatteryPresentCmd);
    appComm.registerRxListener(mExitFunctionalTestsCmd);
    appComm.registerRxListener(mGetPressedKeysCmd);
    appComm.registerRxListener(mDisplayCheckerBoardCmd);
    appComm.registerRxListener(mReceiveDataCmd);
    appComm.registerRxListener(mVersionsCmd);

    mReceiveDataCmd.registerStreamDataListener(mReceiveEventLogCmd);
}

void FunctionalTests::notification(const TestBenchStatus& status)
{
    mExitFunctionalTestsSignal.setData(status);
}

TestBenchStatus FunctionalTests::run()
{
    TestBenchStatus ret;

    mExitFunctionalTestsCmd.add_observer(*this);

    do
    {
        m_hardwareAssembly.getWatchdogItem().refresh();
    } while(!mExitFunctionalTestsSignal.waitData(WAIT_FUNCTIONAL_TESTS_VALIDATION_TIMEOUT_MS, ret));

    mGetPressedKeysCmd.deinit();

    return ret;
}