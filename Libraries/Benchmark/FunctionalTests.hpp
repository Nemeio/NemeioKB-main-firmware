/*
 * FunctionalTests.hpp
 *
 *  Created on: 1 juin 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_FUNCTIONAL_FUNCTIONALTESTS_HPP_
#define BENCHMARK_FUNCTIONAL_FUNCTIONALTESTS_HPP_

#include <array>
#include "AssemblyHardware.h"
#include "Signal.hpp"
#include "PackageFileSystem.hpp"
#include "AppCommProxy.hpp"
#include "SetLedCmd.hpp"
#include "SetFrontlightPowerCmd.hpp"
#include "CheckComponentsCmd.hpp"
#include "TAPLog.hpp"
#include "SetAdvertisingCmd.hpp"
#include "GetTestBenchIdCmd.hpp"
#include "DevicePropertiesProvisioningCmd.hpp"
#include "CheckIfBatteryPresentCmd.hpp"
#include "ExitFunctionalTestsCmd.hpp"
#include "ExitFunctionalTestsCmdObserver.hpp"
#include "GetPressedKeysCmd.hpp"
#include "DisplayCheckerBoardCmd.hpp"
#include "EventLogFileDataTransport.hpp"
#include "ReceiveDataCmd.hpp"
#include "ReceiveEventLogCmd.hpp"
#include "VersionsCmd.hpp"
#include "ClearScreenCmd.hpp"

namespace benchmark
{
class FunctionalTests : public ExitFunctionalTestsCmdObserver
{
public:
    explicit FunctionalTests(AssemblyHardware& hardwareAssembly,
                             AppCommProxy& appComm,
                             Logger& logger,
                             fs::IFileSystem& eventLogFileSystem,
                             std::string_view eventLogFilePath,
                             PackageFileSystem& firmwarePackageFileSystem,
                             DeviceProperties& deviceProperties);
    ~FunctionalTests() final = default;

    TestBenchStatus run();
    void notification(const TestBenchStatus& status) final;

private:
    static constexpr uint32_t WAIT_FUNCTIONAL_TESTS_VALIDATION_TIMEOUT_MS = 4000;

    AssemblyHardware& m_hardwareAssembly;
    EventLogFileDataTransport m_logDataTransport;
    PackageFileSystem& m_firmwarePackageFileSystem;

    TAPLog m_log;

    // SetLedCmd
    SetLedCmd mSetLedCmd;

    // Set Frontlight power
    SetFrontlightPowerCmd mSetFrontlightPowerCmd;

    // Check components
    CheckComponentsCmd mCheckComponentsCmd;

    // ClearScreenCmd
    ClearScreenCmd mClearScreenCmd;

    // Set advertising
    SetAdvertisingCmd mSetAdvertisingCmd;

    // GetTestBenchIdCmd
    GetTestBenchIdCmd mGetTestBenchIdCmd;

    // DevicePropertiesProvisioningCmd
    DevicePropertiesProvisioningCmd mDevicePropertiesProvisioningCmd;

    // CheckIfBatteryPresentCmd
    CheckIfBatteryPresentCmd mCheckIfBatteryPresentCmd;

    // ExitFunctionalTestsCmd
    ExitFunctionalTestsCmd mExitFunctionalTestsCmd;
    DataSignal<TestBenchStatus, 1> mExitFunctionalTestsSignal;

    // GetPressedKeysCmd
    TestBenchKeypad mTestBenchKeypad;
    GetPressedKeysCmd mGetPressedKeysCmd;

    // DisplayCheckerBoardCmd
    CheckerBoard mCheckerBoard;
    DisplayCheckerBoardCmd mDisplayCheckerBoardCmd;

    // ReceiveEventLogCmd
    ReceiveDataCmd mReceiveDataCmd;
    ReceiveEventLogCmd mReceiveEventLogCmd;

    // VersionsCmd
    VersionsCmd mVersionsCmd;
};

} /* namespace benchmark */

#endif /* BENCHMARK_FUNCTIONAL_FUNCTIONALTESTS_HPP_ */
