/*
 * ElectricalTests.hpp
 *
 *  Created on: 7 mai 2021
 *      Author: thomas
 */

#ifndef ELECTRICALTESTS_HPP_
#define ELECTRICALTESTS_HPP_

#include "AssemblyHardware.h"
#include "DataSignal.hpp"
#include "PackageFileSystem.hpp"
#include "AppCommProxy.hpp"
#include "SetLedCmd.hpp"
#include "ExitElectricalTestsCmd.hpp"
#include "ExitElectricalTestsCmdObserver.hpp"
#include "GetBatteryElectricalStatusCmd.hpp"
#include "SetFrontlightPowerCmd.hpp"
#include "CheckComponentsCmd.hpp"
#include "TAPLog.hpp"
#include "ClearScreenCmd.hpp"
#include "SetTestBenchIdCmd.hpp"
#include "GetTestBenchIdCmd.hpp"
#include "GetPressedKeysCmd.hpp"
#include "EventLogFileDataTransport.hpp"
#include "ReceiveDataCmd.hpp"
#include "ReceiveEventLogCmd.hpp"
#include "VersionsCmd.hpp"

namespace benchmark
{
class ElectricalTests : public ExitElectricalTestsCmdObserver
{
public:
    explicit ElectricalTests(AssemblyHardware& hardwareAssembly,
                             AppCommProxy& appComm,
                             Logger& logger,
                             fs::IFileSystem& eventLogFileSystem,
                             std::string_view eventLogFilePath,
                             PackageFileSystem& firmwarePackageFileSystem,
                             TestBenchId& testBenchId);
    ~ElectricalTests() final = default;

    TestBenchStatus run();

private:
    static constexpr uint32_t WAIT_ELECTRICAL_TESTS_VALIDATION_TIMEOUT_MS = 4000;
    static constexpr uint32_t DELAY_PROVISIONING_TIMEOUT_MS = 5000;

    AssemblyHardware& m_hardwareAssembly;
    EventLogFileDataTransport m_logDataTransport;
    PackageFileSystem& m_firmwarePackageFileSystem;

    TAPLog m_log;

    // SetLedCmd
    SetLedCmd mSetLedCmd;

    // ExitElectricalTestsCmd
    ExitElectricalTestsCmd mExitElectricalTestsCmd;
    DataSignal<TestBenchStatus, 1> mExitElectricalTestsSignal;

    void notification(const TestBenchStatus& status) final;

    // GetBatteryElectricalStatusCmd
    GetBatteryElectricalStatusCmd mGetBatteryElectricalStatusCmd;

    // Set Frontlight power
    SetFrontlightPowerCmd mSetFrontlightPowerCmd;

    // Check components
    CheckComponentsCmd mCheckComponentsCmd;

    // ClearScreenCmd
    ClearScreenCmd mClearScreenCmd;

    // SetTestBenchId
    SetTestBenchIdCmd mSetTestBenchIdCmd;

    // GetTestBenchIdCmd
    GetTestBenchIdCmd mGetTestBenchIdCmd;

    // GetPressedKeysCmd
    TestBenchKeypad mTestBenchKeypad;
    GetPressedKeysCmd mGetPressedKeysCmd;

    // ReceiveEventLogCmd
    ReceiveDataCmd mReceiveDataCmd;
    ReceiveEventLogCmd mReceiveEventLogCmd;

    // VersionsCmd
    VersionsCmd mVersionsCmd;
};

} // namespace benchmark

#endif /* ELECTRICALTESTS_HPP_ */
