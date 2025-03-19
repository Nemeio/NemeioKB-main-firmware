/*
 * DeviceProperties.hpp
 *
 *  Created on: 12 juil. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_IDENTIFICATION_DEVICEPROPERTIES_HPP_
#define NEMEIO_IDENTIFICATION_DEVICEPROPERTIES_HPP_

#include "IDevicePropertiesStorage.hpp"
#include "utils.hpp"

class DeviceProperties
{
public:
    DeviceProperties() = delete;
    explicit DeviceProperties(IDevicePropertiesStorage& storage);
    virtual ~DeviceProperties() = default;

    bool setTestBenchId(const TestBenchId& testBenchId);
    bool setProvisioningData(const SerialNumber& serialNumber, const MacAddress& macAddress);

    const TestBenchId& getTestBenchId() const;
    const SerialNumber& getSerialNumber() const;
    const MacAddress& getBleMacAddress() const;

    bool isProvisioned() const;

private:
    static const SerialNumber k_defaultSerialNumber;
    static const MacAddress k_defaultBleMacAddress;
    IDevicePropertiesStorage& m_storage;

    TestBenchId loadTestBenchId(IDevicePropertiesStorage& storage);
    SerialNumber loadSerialNumber(IDevicePropertiesStorage& storage);
    MacAddress loadBleMacAddress(IDevicePropertiesStorage& storage);

    TestBenchId m_testBenchId;
    SerialNumber m_serialNumber;
    MacAddress m_bleMacAddress;
};

#endif /* NEMEIO_IDENTIFICATION_DEVICEPROPERTIES_HPP_ */
