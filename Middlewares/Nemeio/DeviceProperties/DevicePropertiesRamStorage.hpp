#pragma once

#include "IDevicePropertiesStorage.hpp"

class DevicePropertiesRamStorage : public IDevicePropertiesStorage
{
public:
    DevicePropertiesRamStorage() = default;
    ~DevicePropertiesRamStorage() final = default;

    bool saveTestBenchId(const TestBenchId& testBenchId) final;
    bool saveProvisioningData(const SerialNumber& serialNumber, const MacAddress& macAddress) final;
    bool loadTestBenchId(TestBenchId& testBenchId) final;
    bool loadSerialNumber(SerialNumber& serialNumber) final;
    bool loadMacAddress(MacAddress& macAddress) final;

private:
    TestBenchId m_testBenchId;
    SerialNumber m_serialNumber{etl::string<SerialNumber::SIZE>{""}};
    MacAddress m_macAddress{etl::array<uint8_t, MacAddress::SIZE>{0}};
};