#include "DevicePropertiesEnvStorage.hpp"

#ifndef DEVICE_PROPERTIES_DEBUG_STORAGE
#error "DEVICE_PROPERTIES_DEBUG_STORAGE must be defined"
#endif // DEVICE_PROPERTIES_DEBUG_STORAGE

DevicePropertiesEnvStorage::DevicePropertiesEnvStorage(FirmwareEnvironment& env)
    : m_env(env)
{
}

bool DevicePropertiesEnvStorage::saveTestBenchId(const TestBenchId& testBenchId)
{
    m_env.setDevTestBenchId(testBenchId);

    return true;
}

bool DevicePropertiesEnvStorage::saveProvisioningData(const SerialNumber& serialNumber,
                                                      const MacAddress& macAddress)
{
    m_env.setDevSerialNumber(serialNumber);
    m_env.setDevBleMacAddress(macAddress);

    return true;
}

bool DevicePropertiesEnvStorage::loadTestBenchId(TestBenchId& testBenchId)
{
    testBenchId = m_env.getDevTestBenchId();

    return true;
}

bool DevicePropertiesEnvStorage::loadSerialNumber(SerialNumber& serialNumber)
{
    serialNumber = m_env.getDevSerialNumber();

    return true;
}

bool DevicePropertiesEnvStorage::loadMacAddress(MacAddress& macAddress)
{
    macAddress = m_env.getDevBleMacAddress();

    return true;
}
