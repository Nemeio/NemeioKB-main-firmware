#include "DevicePropertiesRamStorage.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class DevicePropertiesRamStorageTest : public ::testing::Test
{
protected:
    DevicePropertiesRamStorageTest() = default;

    TestBenchId stubTestBenchId{"#This is a test bench id for tests"};
    SerialNumber stubSerialNumber{"ABCDEFG12345"};
    MacAddress stubMacAddress{
        etl::array<uint8_t, MacAddress::SIZE>{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}};

    DevicePropertiesRamStorage storage;
};

TEST_F(DevicePropertiesRamStorageTest, Nominal)
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