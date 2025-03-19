#include "BleSetDevicePropertiesCmd.hpp"

BleSetDevicePropertiesCmd::BleSetDevicePropertiesCmd()
    : BleTxCmd(BLESPICommands::CommandId::SET_DEVICE_PROPERTIES)
{
}

void BleSetDevicePropertiesCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(
                                    m_deviceProperties.software_revision),
                                strnlen(m_deviceProperties.software_revision,
                                        sizeof(m_deviceProperties.software_revision))});
}