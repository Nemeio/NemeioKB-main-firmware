/*
 * ElectricalTests.cpp
 *
 *  Created on: 7 mai 2021
 *      Author: thomas
 */

#include "ElectricalTests.hpp"

#include "TAPLog.hpp"

using namespace benchmark;

ElectricalTests::ElectricalTests(AssemblyHardware& hardwareAssembly,
                                 AppCommProxy& appComm,
                                 Logger& logger,
                                 fs::IFileSystem& eventLogFileSystem,
                                 std::string_view eventLogFilePath,
                                 PackageFileSystem& firmwarePackageFileSystem,
                                 TestBenchId& testBenchId)
    : m_hardwareAssembly(hardwareAssembly)
    , m_logDataTransport(logger)
    , m_firmwarePackageFileSystem(firmwarePackageFileSystem)
    , m_log(m_logDataTransport)
    , mSetLedCmd(appComm, m_hardwareAssembly)
    , mExitElectricalTestsCmd(appComm)
    , mGetBatteryElectricalStatusCmd(appComm, m_hardwareAssembly)
    , mSetFrontlightPowerCmd(appComm, m_hardwareAssembly)
    , mCheckComponentsCmd(appComm, m_hardwareAssembly, m_log, m_firmwarePackageFileSystem)
    , mClearScreenCmd(appComm, m_hardwareAssembly.getScreenItem())
    , mSetTestBenchIdCmd(appComm, testBenchId)
    , mGetTestBenchIdCmd(appComm, testBenchId)
    , mTestBenchKeypad(m_hardwareAssembly)
    , mGetPressedKeysCmd(appComm, mTestBenchKeypad)
    , mReceiveDataCmd(appComm)
    , mReceiveEventLogCmd(eventLogFileSystem, eventLogFilePath)
    , mVersionsCmd(appComm,
                   m_hardwareAssembly.getBleItem(),
                   m_hardwareAssembly.getScreenItem().getAssociatedScreenController())
{
    appComm.registerRxListener(mSetLedCmd);
    appComm.registerRxListener(mExitElectricalTestsCmd);
    appComm.registerRxListener(mGetBatteryElectricalStatusCmd);
    appComm.registerRxListener(mSetFrontlightPowerCmd);
    appComm.registerRxListener(mCheckComponentsCmd);
    appComm.registerRxListener(mClearScreenCmd);
    appComm.registerRxListener(mSetTestBenchIdCmd);
    appComm.registerRxListener(mGetTestBenchIdCmd);
    appComm.registerRxListener(mGetPressedKeysCmd);
    appComm.registerRxListener(mReceiveDataCmd);
    appComm.registerRxListener(mVersionsCmd);

    mReceiveDataCmd.registerStreamDataListener(mReceiveEventLogCmd);
}

void ElectricalTests::notification(const TestBenchStatus& status)
{
    mExitElectricalTestsSignal.setData(status);
}

TestBenchStatus ElectricalTests::run()
{
    TestBenchStatus ret;
    TAPLog log(m_logDataTransport);

    mExitElectricalTestsCmd.add_observer(*this);

    do
    {
        m_hardwareAssembly.getWatchdogItem().refresh();
    } while(!mExitElectricalTestsSignal.waitData(WAIT_ELECTRICAL_TESTS_VALIDATION_TIMEOUT_MS, ret));

    mGetPressedKeysCmd.deinit();

    return ret;
}
