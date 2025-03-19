#include "BleSetRadioModeCmd.hpp"

BleSetRadioModeCmd::BleSetRadioModeCmd()
    : BleTxCmd(BLESPICommands::CommandId::SET_RADIO_MODE)
{
}

void BleSetRadioModeCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(&m_mode), sizeof(m_mode)});
}