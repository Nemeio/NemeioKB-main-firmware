#include "BleSetAdvertisingCmd.hpp"

BleSetAdvertisingCmd::BleSetAdvertisingCmd()
    : BleTxCmd(BLESPICommands::CommandId::SET_ADVERTISING)
{
}

void BleSetAdvertisingCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(&m_enable), sizeof(m_enable)});
}