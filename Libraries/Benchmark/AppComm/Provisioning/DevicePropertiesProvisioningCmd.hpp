/*
 * SerialNumberCmd.hpp
 *
 *  Created on: Jul 3, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SERIALNUMBER_SERIALNUMBERPROVISIONINGCMD_HPP_
#define APPCOMM_SERIALNUMBER_SERIALNUMBERPROVISIONINGCMD_HPP_

#include "AppCommCmd.hpp"
#include "DeviceProperties.hpp"
#include <cstring>

namespace benchmark
{
class DevicePropertiesProvisioningCmd : public AppCommCmd
{
public:
    DevicePropertiesProvisioningCmd(IAppCommTxListener& appCommTxListener,
                                    DeviceProperties& deviceProperties);
    virtual ~DevicePropertiesProvisioningCmd() final = default;

private:
    DeviceProperties& m_deviceProperties;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
};

} // namespace benchmark

#endif /* APPCOMM_SERIALNUMBER_SERIALNUMBERPROVISIONINGCMD_HPP_ */
