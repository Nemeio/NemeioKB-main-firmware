#include "BleNumericComparisonCmd.hpp"

BleNumericComparisonCmd::BleNumericComparisonCmd()
    : BleTxRxCmd(BLESPICommands::CommandId::NUMERIC_COMPARISON)
{
}

void BleNumericComparisonCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(&m_accept), sizeof(m_accept)});
}

void BleNumericComparisonCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    if(BLEDefs::PASSKEY_SIZE == dataLen)
    {
        notify_observers(BleNumericComparisonEvent{BLEDefs::Passkey(pData, pData + dataLen)});
    }
}