#pragma once

#include "BleTxCmd.hpp"
#include "BLEDefs.hpp"
#include "etl/string.h"

class BleSetDeviceNameCmd : public BleTxCmd
{
public:
    BleSetDeviceNameCmd();
    virtual ~BleSetDeviceNameCmd() = default;

    void setDeviceName(etl::string_view name)
    {
        m_name = etl::string<BLEDefs::DEVICE_NAME_MAX_SIZE>(name);
    }
    void fill(BleCmdPacket& packet) const final;

private:
    etl::string<BLEDefs::DEVICE_NAME_MAX_SIZE> m_name;
};