#pragma once

#include "IDevicePropertiesStorage.hpp"
#include "FirmwareEnvironment.hpp"

class DevicePropertiesEnvStorage : public IDevicePropertiesStorage
{
public:
    explicit DevicePropertiesEnvStorage(FirmwareEnvironment& env);
    ~DevicePropertiesEnvStorage() final = default;

    bool saveTestBenchId(const TestBenchId& testBenchId) final;
    bool saveProvisioningData(const SerialNumber& serialNumber, const MacAddress& macAddress) final;
    bool loadTestBenchId(TestBenchId& testBenchId) final;
    bool loadSerialNumber(SerialNumber& serialNumber) final;
    bool loadMacAddress(MacAddress& macAddress) final;

private:
    FirmwareEnvironment& m_env;
};
