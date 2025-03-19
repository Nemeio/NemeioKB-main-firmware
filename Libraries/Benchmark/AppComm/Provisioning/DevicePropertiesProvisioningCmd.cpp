/*
 * SerialNumberCmd.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: Labo
 */

#include <DevicePropertiesProvisioningCmd.hpp>
#include "assertTools.h"
#include "SerialNumber.hpp"
#include "MacAddress.hpp"
#include "etl_string_utils.hpp"

using namespace benchmark;

DevicePropertiesProvisioningCmd::DevicePropertiesProvisioningCmd(
    IAppCommTxListener& appCommTxListener, DeviceProperties& deviceProperties)
    : AppCommCmd(AppCommDefs::AppCommCmd::PROVISIONING_MAC_SERIALNUMBER, appCommTxListener)
    , m_deviceProperties(deviceProperties)
{
}

void DevicePropertiesProvisioningCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode ret = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(dataLen == SerialNumber::SIZE + MacAddress::SIZE)
    {
        etl::string<SerialNumber::SIZE> serialNumber(etl_string_utils::arrayToStringView(
            {reinterpret_cast<const char*>(pData),
             reinterpret_cast<const char*>(pData + SerialNumber::SIZE)}));

        pData += SerialNumber::SIZE;

        etl::array<uint8_t, MacAddress::SIZE> macAddress;

        std::copy(pData, pData + MacAddress::SIZE, macAddress.data());

        m_deviceProperties.setProvisioningData(SerialNumber{serialNumber}, MacAddress{macAddress});

        ret = AppCommDefs::AppCommErrCode::APPCOMMERR_SUCCESS;
    }

    uint8_t res = static_cast<uint8_t>(ret);
    transmitResponse(&res, sizeof(res));
}
