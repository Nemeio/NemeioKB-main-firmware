#pragma once

#include "BleTxCmd.hpp"
#include "strlen.h"
#include "BLEDefs.hpp"

class BleSetDevicePropertiesCmd : public BleTxCmd
{
public:
    BleSetDevicePropertiesCmd();
    virtual ~BleSetDevicePropertiesCmd() = default;

    void setDeviceProperties(const BLEDefs::bleDeviceProperties& deviceProperties)
    {
        // String must be null-terminated
        if(strnlen(deviceProperties.software_revision, sizeof(deviceProperties.software_revision))
           < sizeof(deviceProperties.software_revision))
        {
            m_deviceProperties = deviceProperties;
        }
    }
    void fill(BleCmdPacket& packet) const final;

private:
    BLEDefs::bleDeviceProperties m_deviceProperties{0};
};