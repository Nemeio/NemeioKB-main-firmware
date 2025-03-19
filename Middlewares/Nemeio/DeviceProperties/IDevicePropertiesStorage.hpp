#pragma once

#include "TestBenchId.hpp"
#include "SerialNumber.hpp"
#include "MacAddress.hpp"

class IDevicePropertiesStorage
{
public:
    IDevicePropertiesStorage() = default;
    virtual ~IDevicePropertiesStorage() = default;

    virtual bool saveTestBenchId(const TestBenchId& testBenchId) = 0;
    virtual bool saveProvisioningData(const SerialNumber& serialNumber,
                                      const MacAddress& macAddress) = 0;
    virtual bool loadTestBenchId(TestBenchId& testBenchId) = 0;
    virtual bool loadSerialNumber(SerialNumber& serialNumber) = 0;
    virtual bool loadMacAddress(MacAddress& macAddress) = 0;
};