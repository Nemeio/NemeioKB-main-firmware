/*
 * CheckComponentsCmd.hpp
 *
 *  Created on: 14 novembre 2022
 *      Author: Olivier
 */

#ifndef BENCHMARK_COMMON_CHECKCOMPONENTS_CHECKCOMPONENTSCMD_HPP_
#define BENCHMARK_COMMON_CHECKCOMPONENTS_CHECKCOMPONENTSCMD_HPP_

#include "AppCommCmd.hpp"
#include "CheckComponentsCmd.hpp"
#include "IOTesting.hpp"
#include "ComponentsTesting.hpp"

namespace benchmark
{
class CheckComponentsCmd : public AppCommCmd
{
public:
    explicit CheckComponentsCmd(IAppCommTxListener& appCommTxListener,
                                AssemblyHardware& hardwareAssembly,
                                ITestLog& log,
                                PackageFileSystem& firmwarePackageFileSystem);
    ~CheckComponentsCmd() final = default;

private:
    ComponentsTesting mComponentsTesting;
    IOTesting mIOTesting;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);

    enum class SubTestIndexes : uint8_t
    {
        SUBTEST_NRF_COMMUNICATION,
        SUBTEST_ITE_COMMUNICATION,
        SUBTEST_CHECK_ITEFLASH,
        SUBTEST_CHECK_EXTERNALFLASH,
        SUBTEST_STM32VERSION,
        SUBTEST_CHECK_BATTERYCHARGER,
        SUBTEST_CHECK_MATRIXIO,
    };

    struct SubtestToTestsuite
    {
        std::function<bool(uint8_t)> singleTestSuiteFunction;
        uint8_t singleTestSuiteIndex;
    };

    etl::array<SubtestToTestsuite, 7> componentsTest = {{
        // SUBTEST_NRF_COMMUNICATION
        {std::bind(&ComponentsTesting::runSingleTest, mComponentsTesting, std::placeholders::_1),
         static_cast<uint8_t>(
             ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_NRF_COMMUNICATION)},
        // SUBTEST_ITE_COMMUNICATION
        {std::bind(&ComponentsTesting::runSingleTest, mComponentsTesting, std::placeholders::_1),
         static_cast<uint8_t>(
             ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_ITE_COMMUNICATION)},
        // SUBTEST_CHECK_ITEFLASH
        {std::bind(&IOTesting::runSingleTest, mIOTesting, std::placeholders::_1),
         static_cast<uint8_t>(IOTesting::IOTestingIndexes::SINGLE_IOTESTING_CHECK_ITEFLASH)},
        // SUBTEST_CHECK_EXTERNALFLASH
        {std::bind(&IOTesting::runSingleTest, mIOTesting, std::placeholders::_1),
         static_cast<uint8_t>(IOTesting::IOTestingIndexes::SINGLE_IOTESTING_CHECK_EXTERNALFLASH)},
        // SUBTEST_STM32VERSION
        {std::bind(&ComponentsTesting::runSingleTest, mComponentsTesting, std::placeholders::_1),
         static_cast<uint8_t>(
             ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_STM32VERSION)},
        // SUBTEST_CHECK_BATTERYCHARGER
        {std::bind(&ComponentsTesting::runSingleTest, mComponentsTesting, std::placeholders::_1),
         static_cast<uint8_t>(ComponentsTesting::ComponentsTestingIndexes::
                                  SINGLE_COMPONENTSTESTING_CHECK_BATTERYCHARGER)},
        // SUBTEST_CHECK_MATRIXIO
        {std::bind(&IOTesting::runSingleTest, mIOTesting, std::placeholders::_1),
         static_cast<uint8_t>(IOTesting::IOTestingIndexes::SINGLE_IOTESTING_CHECK_MATRIXIO)},
    }};
};

} // namespace benchmark

#endif /* BENCHMARK_COMMON_CHECKCOMPONENTS_CHECKCOMPONENTSCMD_HPP_ */
