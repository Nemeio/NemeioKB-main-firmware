#include "BleHidReportDescriptorCmd.hpp"
#include "utils.hpp"

BleHidReportDescriptorCmd::BleHidReportDescriptorCmd()
    : BleTxCmd(BLESPICommands::CommandId::UPDATE_REPORT_DESC)
{
}

void BleHidReportDescriptorCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    // m_reportLengths cannot be > MAX_NB_REPORTS (uint8_t), so cast if safe
    auto nbReportLengths = static_cast<uint8_t>(m_reportLengths.size());
    packet.addParameter(BleCmdPacket::Parameter{&nbReportLengths, sizeof(nbReportLengths)});

    etl::vector<uint16_t, MAX_NB_REPORTS> m_reportLengthsBE16;
    for(size_t i = 0; i < m_reportLengths.size(); ++i)
    {
        m_reportLengthsBE16.push_back(HostToBE16(m_reportLengths[i]));
    }
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(m_reportLengthsBE16.data()),
                                sizeof(uint16_t) * m_reportLengthsBE16.size()});

    packet.addParameter(BleCmdPacket::Parameter{m_reportMap.begin(), m_reportMap.size()});
}