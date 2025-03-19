#include "DevicePropertiesEnvStorage.hpp"
#include "FirmwareEnvironmentTest.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace firmware_environment_test;

class DevicePropertiesEnvStorageTest : public ::testing::Test
{
protected:
    DevicePropertiesEnvStorageTest()
        : env(createEnv(stubEnvSettings))
        , storage(env)
    {
    }

    FirmwareEnvironment createEnv(const envSettings_t& envSettings)
    {
        std::array<std::byte, sizeof(envSettings_t)> inputArray;

        memcpy(inputArray.data(), &envSettings, sizeof(envSettings));
        FirmwareEnvironment retEnv(etl::array_view(inputArray.data(), inputArray.size()));

        return retEnv;
    }

    TestBenchId stubTestBenchId{"#This is a test bench id for tests"};
    SerialNumber stubSerialNumber{"ABCDEFG12345"};
    MacAddress stubMacAddress{
        etl::array<uint8_t, MacAddress::SIZE>{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}};
    envSettings_t stubEnvSettings = {
        .version = LATEST_VERSION,
        .latest = {.nextFirmwareUpdateBank = FirmwareUpdateBank::UNKNOWN,
                   .updateInProgress = false,
                   .electricalTestsPassed = false,
                   .functionalTestsPassed = false,
                   .fsVersion = 1,
                   .devTestBenchId = {},
                   .devBleMacAddress = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC},
                   .devSerialNumber = {'A', 'B', 'C', 'D', 'E', 'F', 'G', '1', '2', '3', '4', '5'}},
    };

    FirmwareEnvironment env;
    DevicePropertiesEnvStorage storage;
};

TEST_F(DevicePropertiesEnvStorageTest, Nominal)
{
    TestBenchId loadedTestBenchId;
    SerialNumber loadedSerialNumber{etl::string<SerialNumber::SIZE>(SerialNumber::SIZE, '0')};
    MacAddress loadedMacAddress{etl::array<uint8_t, MacAddress::SIZE>{0}};

    EXPECT_TRUE(storage.saveTestBenchId(stubTestBenchId));
    EXPECT_TRUE(storage.saveProvisioningData(stubSerialNumber, stubMacAddress));
    EXPECT_TRUE(storage.loadTestBenchId(loadedTestBenchId));
    EXPECT_TRUE(storage.loadSerialNumber(loadedSerialNumber));
    EXPECT_TRUE(storage.loadMacAddress(loadedMacAddress));

    EXPECT_EQ(loadedTestBenchId, stubTestBenchId);
    EXPECT_EQ(loadedSerialNumber, stubSerialNumber);
    EXPECT_EQ(loadedMacAddress, stubMacAddress);
}