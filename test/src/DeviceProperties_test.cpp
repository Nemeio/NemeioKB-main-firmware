#include "DeviceProperties.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "MockDevicePropertiesStorage.hpp"

using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::StrictMock;

class DevicePropertiesTest : public ::testing::Test
{
protected:
    DevicePropertiesTest() {}

    TestBenchId stubTestBenchId{"#This is a test bench id for tests"};
    SerialNumber stubSerialNumber{"ABCDEFG12345"};
    MacAddress stubMacAddress{
        etl::array<uint8_t, MacAddress::SIZE>{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}};

    TestBenchId invalidTestBenchID{"a"};
    SerialNumber invalidSerialNumber{""};

    TestBenchId defaultTestBenchId;
    SerialNumber defaultSerialNumber{"000000000000"};
    MacAddress defaultMacAddress{etl::array<uint8_t, MacAddress::SIZE>{0}};

    StrictMock<MockDevicePropertiesStorage> storage;
};

TEST_F(DevicePropertiesTest, LoadNominal)
{
    EXPECT_CALL(storage, loadTestBenchId)
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(stubTestBenchId), Return(true)));
    EXPECT_CALL(storage, loadSerialNumber)
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(stubSerialNumber), Return(true)));
    EXPECT_CALL(storage, loadMacAddress)
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(stubMacAddress), Return(true)));

    DeviceProperties deviceProperties(storage);

    EXPECT_EQ(stubTestBenchId, deviceProperties.getTestBenchId());
    EXPECT_EQ(stubSerialNumber, deviceProperties.getSerialNumber());
    EXPECT_EQ(stubMacAddress, deviceProperties.getBleMacAddress());
    EXPECT_TRUE(deviceProperties.isProvisioned());
}

TEST_F(DevicePropertiesTest, LoadFail)
{
    EXPECT_CALL(storage, loadTestBenchId).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadSerialNumber).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadMacAddress).Times(1).WillOnce(Return(false));

    DeviceProperties deviceProperties(storage);

    EXPECT_EQ(defaultTestBenchId, deviceProperties.getTestBenchId());
    EXPECT_EQ(defaultSerialNumber, deviceProperties.getSerialNumber());
    EXPECT_EQ(defaultMacAddress, deviceProperties.getBleMacAddress());
    EXPECT_FALSE(deviceProperties.isProvisioned());
}

TEST_F(DevicePropertiesTest, LoadSuccessInvalidData)
{
    EXPECT_CALL(storage, loadTestBenchId)
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(invalidTestBenchID), Return(true)));
    EXPECT_CALL(storage, loadSerialNumber)
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(invalidSerialNumber), Return(true)));
    EXPECT_CALL(storage, loadMacAddress)
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(defaultMacAddress), Return(true)));

    DeviceProperties deviceProperties(storage);

    EXPECT_EQ(defaultTestBenchId, deviceProperties.getTestBenchId());
    EXPECT_EQ(defaultSerialNumber, deviceProperties.getSerialNumber());
    EXPECT_EQ(defaultMacAddress, deviceProperties.getBleMacAddress());
    EXPECT_FALSE(deviceProperties.isProvisioned());
}

TEST_F(DevicePropertiesTest, SetPropertiesNominal)
{
    EXPECT_CALL(storage, loadTestBenchId).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadSerialNumber).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadMacAddress).Times(1).WillOnce(Return(false));

    DeviceProperties deviceProperties(storage);

    EXPECT_CALL(storage, saveTestBenchId(stubTestBenchId)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(deviceProperties.setTestBenchId(stubTestBenchId));

    EXPECT_CALL(storage, saveProvisioningData(stubSerialNumber, stubMacAddress))
        .Times(1)
        .WillOnce(Return(true));
    EXPECT_TRUE(deviceProperties.setProvisioningData(stubSerialNumber, stubMacAddress));

    EXPECT_EQ(deviceProperties.getTestBenchId(), stubTestBenchId);
    EXPECT_EQ(deviceProperties.getSerialNumber(), stubSerialNumber);
    EXPECT_EQ(deviceProperties.getBleMacAddress(), stubMacAddress);
}

TEST_F(DevicePropertiesTest, SetPropertiesInvalid)
{
    EXPECT_CALL(storage, loadTestBenchId).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadSerialNumber).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadMacAddress).Times(1).WillOnce(Return(false));

    DeviceProperties deviceProperties(storage);

    EXPECT_CALL(storage, saveTestBenchId).Times(0);
    EXPECT_FALSE(deviceProperties.setTestBenchId(invalidTestBenchID));

    EXPECT_CALL(storage, saveProvisioningData).Times(0);
    EXPECT_FALSE(deviceProperties.setProvisioningData(invalidSerialNumber, stubMacAddress));

    EXPECT_EQ(defaultTestBenchId, deviceProperties.getTestBenchId());
    EXPECT_EQ(defaultSerialNumber, deviceProperties.getSerialNumber());
    EXPECT_EQ(defaultMacAddress, deviceProperties.getBleMacAddress());
    EXPECT_FALSE(deviceProperties.isProvisioned());
}

TEST_F(DevicePropertiesTest, SetPropertiesSaveFail)
{
    EXPECT_CALL(storage, loadTestBenchId).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadSerialNumber).Times(1).WillOnce(Return(false));
    EXPECT_CALL(storage, loadMacAddress).Times(1).WillOnce(Return(false));

    DeviceProperties deviceProperties(storage);

    EXPECT_CALL(storage, saveTestBenchId(stubTestBenchId)).Times(1).WillOnce(Return(false));
    EXPECT_FALSE(deviceProperties.setTestBenchId(stubTestBenchId));

    EXPECT_CALL(storage, saveProvisioningData(stubSerialNumber, stubMacAddress))
        .Times(1)
        .WillOnce(Return(false));
    EXPECT_FALSE(deviceProperties.setProvisioningData(stubSerialNumber, stubMacAddress));

    EXPECT_EQ(defaultTestBenchId, deviceProperties.getTestBenchId());
    EXPECT_EQ(defaultSerialNumber, deviceProperties.getSerialNumber());
    EXPECT_EQ(defaultMacAddress, deviceProperties.getBleMacAddress());
    EXPECT_FALSE(deviceProperties.isProvisioned());
}