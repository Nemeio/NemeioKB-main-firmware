#include "BleDfuCommitVersionCmd.hpp"

BleDfuCommitVersionCmd::BleDfuCommitVersionCmd()
    : BleTxCmd(BLESPICommands::CommandId::DFU_COMMIT_VERSION)
{
}

void BleDfuCommitVersionCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
    packet.addParameter(
        BleCmdPacket::Parameter{reinterpret_cast<const uint8_t*>(&m_version), sizeof(m_version)});
}