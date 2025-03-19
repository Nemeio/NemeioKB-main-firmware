#pragma once

#include "gmock/gmock.h"
#include "IDevicePropertiesStorage.hpp"

class MockDevicePropertiesStorage : public IDevicePropertiesStorage
{
public:
    MOCK_METHOD(bool, saveTestBenchId, (const TestBenchId& testBenchId), (override));
    MOCK_METHOD(bool,
                saveProvisioningData,
                (const SerialNumber& serialNumber, const MacAddress& macAddress),
                (override));
    MOCK_METHOD(bool, loadTestBenchId, (TestBenchId & testBenchId), (override));
    MOCK_METHOD(bool, loadSerialNumber, (SerialNumber & serialNumber), (override));
    MOCK_METHOD(bool, loadMacAddress, (MacAddress & macAddress), (override));
};
