#include "BleDfuDataCmd.hpp"
#include "assertTools.h"
#include "IBleChip.hpp"

BleDfuDataCmd::BleDfuDataCmd()
    : BleTxRxCmd(BLESPICommands::CommandId::DFU_DATA)
{
}

void BleDfuDataCmd::setData(const uint8_t* data, size_t len)
{
    ASSERT(len <= SLIP_DATA_MAX_SIZE);

    m_data = data;
    m_len = len;
}

void BleDfuDataCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(BleCmdPacket::Parameter{m_data, m_len});
}

void BleDfuDataCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    notify_observers(BleDfuResponse{pData, dataLen});
}