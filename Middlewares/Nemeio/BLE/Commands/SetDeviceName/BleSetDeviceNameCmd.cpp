#include "BleSetDeviceNameCmd.hpp"

BleSetDeviceNameCmd::BleSetDeviceNameCmd()
    : BleTxCmd(BLESPICommands::CommandId::SET_DEVICE_NAME)
{
}

void BleSetDeviceNameCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(m_name.data()), m_name.size()});
}