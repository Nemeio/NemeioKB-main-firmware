#include "BleSetMacAddressCmd.hpp"

BleSetMacAddressCmd::BleSetMacAddressCmd()
    : BleTxCmd(BLESPICommands::CommandId::SET_MAC_ADDRESS)
{
}

void BleSetMacAddressCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    etl::array_view<const uint8_t> macAddressBuffer = m_mac.get();
    packet.addParameter(BleCmdPacket::Parameter{macAddressBuffer.begin(), macAddressBuffer.size()});
}