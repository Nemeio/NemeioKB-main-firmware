#include "BleGetFwVersionCmd.hpp"
#include "IBleChip.hpp"

BleGetFwVersionCmd::BleGetFwVersionCmd()
    : BleTxRequest(BLESPICommands::CommandId::GET_FW_VERSION)
{
}

void BleGetFwVersionCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
}

void BleGetFwVersionCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    if(sizeof(BLEDefs::FirmwareVersion) == dataLen)
    {
        notify_observers(
            BleFwVersionEvent{*reinterpret_cast<const BLEDefs::FirmwareVersion*>(pData)});
    }
}

void BleGetFwVersionCmd::onTimeout()
{
    notify_observers(BleTxRequestTimeout{getId()});
}