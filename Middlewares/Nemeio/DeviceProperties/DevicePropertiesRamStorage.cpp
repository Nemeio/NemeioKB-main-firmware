#include "DevicePropertiesRamStorage.hpp"

bool DevicePropertiesRamStorage::saveTestBenchId(const TestBenchId& testBenchId)
{
    m_testBenchId = testBenchId;

    return true;
}

bool DevicePropertiesRamStorage::saveProvisioningData(const SerialNumber& serialNumber,
                                                      const MacAddress& macAddress)
{
    m_serialNumber = serialNumber;
    m_macAddress = macAddress;

    return true;
}

bool DevicePropertiesRamStorage::loadTestBenchId(TestBenchId& testBenchId)
{
    testBenchId = m_testBenchId;

    return true;
}

bool DevicePropertiesRamStorage::loadSerialNumber(SerialNumber& serialNumber)
{
    serialNumber = m_serialNumber;

    return true;
}

bool DevicePropertiesRamStorage::loadMacAddress(MacAddress& macAddress)
{
    macAddress = m_macAddress;

    return true;
}