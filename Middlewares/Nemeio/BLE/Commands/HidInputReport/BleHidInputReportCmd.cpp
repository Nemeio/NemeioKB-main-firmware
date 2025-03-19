#include "BleHidInputReportCmd.hpp"

BleHidInputReportCmd::BleHidInputReportCmd()
    : BleTxCmd(BLESPICommands::CommandId::SEND_INPUT_REPORT)
{
}

void BleHidInputReportCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(BleCmdPacket::Parameter{&m_reportId, sizeof(m_reportId)});
    packet.addParameter(BleCmdPacket::Parameter{m_report.begin(), m_report.size()});
}