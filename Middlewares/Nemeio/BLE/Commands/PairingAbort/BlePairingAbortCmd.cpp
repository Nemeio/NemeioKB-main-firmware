#include "BlePairingAbortCmd.hpp"

BlePairingAbortCmd::BlePairingAbortCmd()
    : AbstractBleCommandListener(BLESPICommands::CommandId::PAIRING_ABORT)
{
}

void BlePairingAbortCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    notify_observers(BlePairingAbortEvent{});
}