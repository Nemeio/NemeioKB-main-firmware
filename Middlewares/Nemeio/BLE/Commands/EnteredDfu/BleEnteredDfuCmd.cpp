#include "BleEnteredDfuCmd.hpp"

BleEnteredDfuCmd::BleEnteredDfuCmd()
    : AbstractBleCommandListener(BLESPICommands::CommandId::ENTERED_DFU)
{
}

void BleEnteredDfuCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    notify_observers(BleEnteredDfuEvent{});
}