/*
 * SFlashIOCommonInterface.cpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#include <SFlashIOCommonInterface.hpp>

SFlashIOCommonInterface::SFlashIOCommonInterface(
    IFlashConfiguration& flashConfig,
    ISFlashCommunicationInterface& communicationInterface,
    const FlashIOLut& command)
    : mFlashConfiguration(flashConfig)
    , mCommunicationInterface(communicationInterface)
    , mFlashIOInfos(command)
{
}

bool SFlashIOCommonInterface::read(void* buffer, uint32_t addr, size_t size)
{
    FlashCommand flashCommand = {
        .command = static_cast<uint8_t>(mFlashIOInfos.readCommand),
        .addrMode = lutToAddrMode(mFlashIOInfos.addrMode),
        .address = addr,
        .dummy = mFlashIOInfos.readCommandDummy,
        .dataNbLines = mFlashIOInfos.readCommandDataNbLines,
    };

    return mCommunicationInterface.receive(flashCommand,
                                           reinterpret_cast<uint8_t*>(buffer),
                                           size,
                                           true);
}

bool SFlashIOCommonInterface::writePage(uint32_t addr, size_t size, uint8_t* data)
{
    bool ret = false;
    FlashCommand flashCommand = {
        .command = static_cast<uint8_t>(mFlashIOInfos.writePageCommand),
        .addrMode = lutToAddrMode(mFlashIOInfos.addrMode),
        .address = addr,
        .dummy = 0,
        .dataNbLines = mFlashIOInfos.writePageCommandDataNbLines,
    };

    if(!mFlashConfiguration.writeEnable())
    {
        return false;
    }

    ret = mCommunicationInterface.transmit(flashCommand, data, size, true);

    if(ret && !mFlashConfiguration.waitMemReady())
    {
        ret = false;
    }

    return ret;
}

bool SFlashIOCommonInterface::sectorErase(uint32_t addr)
{
    bool ret = false;
    FlashCommand flashCommand = {
        .command = static_cast<uint8_t>(mFlashIOInfos.eraseCommand),
        .addrMode = lutToAddrMode(mFlashIOInfos.addrMode),
        .address = addr,
        .dummy = 0,
        .dataNbLines = 0,
    };

    if(!mFlashConfiguration.writeEnable())
    {
        return false;
    }

    ret = mCommunicationInterface.transmit(flashCommand);

    if(ret && !mFlashConfiguration.waitMemReady())
    {
        ret = false;
    }

    return ret;
}

const FlashInfo SFlashIOCommonInterface::getFlashInfo()
{
    return {0, 0, 0, 0}; /* Not defined at this point */
}

inline SFlashCommunicationInterfaceAddrMode SFlashIOCommonInterface::lutToAddrMode(
    addressingMode addrMode)
{
    SFlashCommunicationInterfaceAddrMode mode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE;

    switch(addrMode)
    {
    case addressingMode::FLASH_3_BYTES_ADDRESSING:
        mode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_3_BYTES;
        break;
    case addressingMode::FLASH_4_BYTES_ADDRESSING:
        mode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_4_BYTES;
        break;
    default:
        break;
    }

    return mode;
}
