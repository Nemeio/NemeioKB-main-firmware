#include "BleSerialDataCmd.hpp"

BleSerialDataCmd::BleSerialDataCmd()
    : BleTxRxCmd(BLESPICommands::CommandId::SERIAL_DATA)
{
}

size_t BleSerialDataCmd::setData(etl::array_view<const uint8_t> data)
{
    size_t chunkSize = std::min(data.size(), MAX_SERIAL_DATA_TX_LEN);
    m_data = etl::array_view<const uint8_t>(data.begin(), chunkSize);
    return chunkSize;
}


void BleSerialDataCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(BleCmdPacket::Parameter{m_data.begin(), m_data.size()});
}

void BleSerialDataCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    notify_observers(BleSerialDataEvent{{pData, dataLen}});
}