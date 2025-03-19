#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "FirmwareEnvironment.hpp"
#include "FirmwareEnvironmentTest.hpp"

using ::testing::NiceMock;
using ::testing::StrictMock;

using namespace firmware_environment_test;

class FirmwareEnvironmentTest : public ::testing::Test
{
protected:
    FirmwareEnvironmentTest(){};
};

TEST_F(FirmwareEnvironmentTest, DefaultValues)
{
    std::array<std::byte, sizeof(envSettings_t)> array;
    FirmwareEnvironment env;

    envSettings_t values = {
        .version = LATEST_VERSION,
        .latest = {.nextFirmwareUpdateBank = FirmwareUpdateBank::UNKNOWN,
                   .updateInProgress = false,
                   .electricalTestsPassed = false,
                   .functionalTestsPassed = false,
                   .fsVersion = FILE_SYSTEM_VERSION,
                   .devTestBenchId = {},
                   .devBleMacAddress = {},
                   .devSerialNumber = {}},
    };

    EXPECT_EQ(array.size(), env.toBytes(etl::array_view(array.data(), array.size())));

    EXPECT_EQ(memcmp(&values, array.data(), array.size()), 0);
};

TEST_F(FirmwareEnvironmentTest, MigrationV4)
{
    std::array<std::byte, sizeof(envSettings_t)> inputArray;
    std::array<std::byte, sizeof(envSettings_t)> array;

    envSettings_t expectedValues = {
        .version = LATEST_VERSION,
        .latest = {.nextFirmwareUpdateBank = FirmwareUpdateBank::BANK_A,
                   .updateInProgress = true,
                   .electricalTestsPassed = true,
                   .functionalTestsPassed = true,
                   .fsVersion = 0,
                   .devTestBenchId = {},
                   .devBleMacAddress = {},
                   .devSerialNumber = {}},
    };

    envSettings_t values = {
        .version = 4,
        .v4 =
            {
                .nextFirmwareUpdateBank = FirmwareUpdateBank::BANK_A,
                .updateInProgress = true,
                .electricalTestsPassed = true,
                .functionalTestsPassed = true,
            },
    };

    memcpy(inputArray.data(), &values, sizeof(values));
    FirmwareEnvironment env(etl::array_view(inputArray.data(), inputArray.size()));


    EXPECT_EQ(inputArray.size(), env.toBytes(etl::array_view(array.data(), array.size())));
    EXPECT_EQ(memcmp(&expectedValues, array.data(), array.size()), 0);
};

TEST_F(FirmwareEnvironmentTest, MigrationV5)
{
    std::array<std::byte, sizeof(envSettings_t)> inputArray;
    std::array<std::byte, sizeof(envSettings_t)> array;

    envSettings_t expectedValues = {
        .version = LATEST_VERSION,
        .latest = {.nextFirmwareUpdateBank = FirmwareUpdateBank::BANK_A,
                   .updateInProgress = true,
                   .electricalTestsPassed = true,
                   .functionalTestsPassed = true,
                   .fsVersion = FILE_SYSTEM_VERSION,
                   .devTestBenchId = {},
                   .devBleMacAddress = {},
                   .devSerialNumber = {}},
    };

    envSettings_t values = {
        .version = 5,
        .v5 =
            {
                .nextFirmwareUpdateBank = FirmwareUpdateBank::BANK_A,
                .updateInProgress = true,
                .electricalTestsPassed = true,
                .functionalTestsPassed = true,
                .fsVersion = FILE_SYSTEM_VERSION,
            },
    };

    memcpy(inputArray.data(), &values, sizeof(values));
    FirmwareEnvironment env(etl::array_view(inputArray.data(), inputArray.size()));


    EXPECT_EQ(inputArray.size(), env.toBytes(etl::array_view(array.data(), array.size())));
    EXPECT_EQ(memcmp(&expectedValues, array.data(), array.size()), 0);
};

TEST_F(FirmwareEnvironmentTest, MigrationOld)
{
    std::array<std::byte, sizeof(envSettings_t)> inputArray;
    std::array<std::byte, sizeof(envSettings_t)> array;

    envSettings_t expectedValues = {
        .version = LATEST_VERSION,
        .latest = {.nextFirmwareUpdateBank = FirmwareUpdateBank::UNKNOWN,
                   .updateInProgress = false,
                   .electricalTestsPassed = false,
                   .functionalTestsPassed = false,
                   .fsVersion = 0,
                   .devTestBenchId = {},
                   .devBleMacAddress = {},
                   .devSerialNumber = {}},
    };

    envSettings_t values = {
        .version = 3,
    };

    memcpy(inputArray.data(), &values, sizeof(values));
    FirmwareEnvironment env(etl::array_view(inputArray.data(), inputArray.size()));


    EXPECT_EQ(inputArray.size(), env.toBytes(etl::array_view(array.data(), array.size())));
    EXPECT_EQ(memcmp(&expectedValues, array.data(), array.size()), 0);
};

TEST_F(FirmwareEnvironmentTest, TestBenchId)
{
    etl::string<TestBenchId::SIZE> testBenchIdStr = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ012345#";
    TestBenchId testBenchId(testBenchIdStr);

    FirmwareEnvironment env;

    env.setDevTestBenchId(testBenchId);

    EXPECT_EQ(env.getDevTestBenchId(), testBenchId);
};

TEST_F(FirmwareEnvironmentTest, SerialNumber)
{
    SerialNumber serialNumber("SerialNumber");

    FirmwareEnvironment env;

    env.setDevSerialNumber(serialNumber);

    EXPECT_EQ(env.getDevSerialNumber(), serialNumber);
};

TEST_F(FirmwareEnvironmentTest, MacAddress)
{
    MacAddress macAddress{etl::array<uint8_t, MacAddress::SIZE>{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}};

    FirmwareEnvironment env;

    env.setDevBleMacAddress(macAddress);

    EXPECT_EQ(env.getDevBleMacAddress(), macAddress);
};