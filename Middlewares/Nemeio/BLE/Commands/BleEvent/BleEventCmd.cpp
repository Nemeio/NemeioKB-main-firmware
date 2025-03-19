#include "BleEventCmd.hpp"

BleEventCmd::BleEventCmd()
    : AbstractBleCommandListener(BLESPICommands::CommandId::BLE_EVENT)
{
}

void BleEventCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    if(sizeof(BleEventValue) == dataLen)
    {
        auto event = static_cast<BleEventValue>(pData[0]);
        notify_observers(BleEventCmdEvent{event});
    }
}