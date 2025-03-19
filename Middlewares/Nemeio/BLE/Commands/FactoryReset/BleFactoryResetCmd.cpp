#include "BleFactoryResetCmd.hpp"

BleFactoryResetCmd::BleFactoryResetCmd()
    : BleTxRequest(BLESPICommands::CommandId::FACTORY_RESET)
{
}

void BleFactoryResetCmd::fill(BleCmdPacket& packet) const
{
    packet.setId(getId());
}

void BleFactoryResetCmd::receivedData(const uint8_t* pData, std::size_t dataLen)
{
    if(sizeof(BLESPICommands::CmdRetCode) == dataLen)
    {
        BLESPICommands::CmdRetCode cmdRetCode = *reinterpret_cast<const BLESPICommands::CmdRetCode*>(
            pData);
        BLEDefs::ErrCode err = BLESPICommands::CmdRetCode::SUCCESS == cmdRetCode
                                   ? BLEDefs::ErrCode::SUCCESS
                                   : BLEDefs::ErrCode::UNKNOWN;

        notify_observers(FactoryResetEvent{err});
    }
}


void BleFactoryResetCmd::onTimeout()
{
    notify_observers(BleTxRequestTimeout{getId()});
}