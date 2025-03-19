/*
 * DeviceProperties.cpp
 *
 *  Created on: 12 juil. 2021
 *      Author: thomas
 */

#include "DeviceProperties.hpp"
#include "assertTools.h"

const SerialNumber DeviceProperties::k_defaultSerialNumber{
    etl::string<SerialNumber::SIZE>(SerialNumber::SIZE, '0')};
const MacAddress DeviceProperties::k_defaultBleMacAddress{etl::array<uint8_t, MacAddress::SIZE>{0}};

DeviceProperties::DeviceProperties(IDevicePropertiesStorage& storage)
    : m_storage(storage)
    , m_testBenchId(loadTestBenchId(storage))
    , m_serialNumber(loadSerialNumber(storage))
    , m_bleMacAddress(loadBleMacAddress(storage))
{
}

bool DeviceProperties::setTestBenchId(const TestBenchId& testBenchId)
{
    bool ret = false;

    if(testBenchId.isValid() && m_storage.saveTestBenchId(testBenchId))
    {
        m_testBenchId = testBenchId;
        ret = true;
    }

    return ret;
}

bool DeviceProperties::setProvisioningData(const SerialNumber& serialNumber,
                                           const MacAddress& macAddress)
{
    bool ret = false;

    if(serialNumber != k_defaultSerialNumber && serialNumber.isValid() && macAddress.isValid()
       && m_storage.saveProvisioningData(serialNumber, macAddress))
    {
        m_serialNumber = serialNumber;
        m_bleMacAddress = macAddress;
        ret = true;
    }

    return ret;
}

const TestBenchId& DeviceProperties::getTestBenchId() const
{
    return m_testBenchId;
}

const SerialNumber& DeviceProperties::getSerialNumber() const
{
    return m_serialNumber;
}

const MacAddress& DeviceProperties::getBleMacAddress() const
{
    return m_bleMacAddress;
}

bool DeviceProperties::isProvisioned() const
{
    return m_serialNumber.isValid() && m_serialNumber != k_defaultSerialNumber
           && m_bleMacAddress.isValid() && m_testBenchId.isValid();
}

TestBenchId DeviceProperties::loadTestBenchId(IDevicePropertiesStorage& storage)
{
    TestBenchId testBenchId;
    storage.loadTestBenchId(testBenchId);

    return testBenchId.isValid() ? testBenchId : TestBenchId();
}

SerialNumber DeviceProperties::loadSerialNumber(IDevicePropertiesStorage& storage)
{
    SerialNumber serialNumber{k_defaultSerialNumber};
    storage.loadSerialNumber(serialNumber);

    return serialNumber.isValid() ? serialNumber : k_defaultSerialNumber;
}

MacAddress DeviceProperties::loadBleMacAddress(IDevicePropertiesStorage& storage)
{
    MacAddress macAddress{k_defaultBleMacAddress};
    storage.loadMacAddress(macAddress);

    return macAddress.isValid() ? macAddress : k_defaultBleMacAddress;
}