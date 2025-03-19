#include "BleNrfStartedCmd.hpp"

BleNrfStartedCmd::BleNrfStartedCmd()
    : AbstractBleCommandListener(BLESPICommands::CommandId::NRF_STARTED)
{
}

void BleNrfStartedCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    if(sizeof(uint8_t) == dataLen)
    {
        notify_observers(BleStartedEvent{*pData});
    }
}