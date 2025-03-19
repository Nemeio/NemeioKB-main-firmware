#include "BleDfuStartCmd.hpp"

BleDfuStartCmd::BleDfuStartCmd()
    : BleTxCmd(BLESPICommands::CommandId::START_DFU)
{
}

void BleDfuStartCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
}