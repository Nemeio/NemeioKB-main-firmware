/*
 * SFlashConfigurationCommonInterface.cpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#include <SFlashConfigurationCommonInterface.hpp>

SFlashConfigurationCommonInterface::SFlashConfigurationCommonInterface(
    ISFlashCommunicationInterface& com)
    : mCommunicationInterface(com)
{
}

bool SFlashConfigurationCommonInterface::readStatusRegisterCmd(uint8_t* data, size_t size)
{
    return readData(OpCode::READ_STATUS_REGISTER_RDSR, data, size);
}

bool SFlashConfigurationCommonInterface::readConfigurationRegisterCmd(uint8_t* data, size_t size)
{
    return readData(OpCode::READ_CONFIG_REGISTER_RDCR, data, size);
}

bool SFlashConfigurationCommonInterface::writeRegistersCmd(uint8_t* data, size_t size)
{
    bool ret = false;
    FlashCommand flashCommand = {
        .command = static_cast<uint8_t>(OpCode::WRITE_REGISTER),
        .addrMode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE,
        .dummy = 0,
        .dataNbLines = 1,
    };

    if(writeEnable())
    {
        ret = mCommunicationInterface.transmit(flashCommand, data, size);
    }

    waitMemReady();

    return ret;
}

uint64_t SFlashConfigurationCommonInterface::getUniqueId()
{
    uint64_t id = 0;

    if(!readData(OpCode::READ_UNIQUE_ID, reinterpret_cast<uint8_t*>(&id), sizeof(id)))
    {
        id = 0;
    }

    return id;
}

uint32_t SFlashConfigurationCommonInterface::getIdentifier()
{
    uint32_t info = 0;
    static constexpr size_t READ_MANUFACTURER_INFO_SIZE = 3;
    static_assert(sizeof(info) >= READ_MANUFACTURER_INFO_SIZE);

    if(!readData(OpCode::READ_MANUFACTURER_AND_DEVICE_IDENTIFICATION,
                 reinterpret_cast<uint8_t*>(&info),
                 3)
       || info == UINT64_MAX)
    {
        info = 0;
    }

    return info;
}

bool SFlashConfigurationCommonInterface::readData(OpCode readRegInstruction,
                                                  uint8_t* data,
                                                  size_t size)
{
    FlashCommand flashCommand = {
        .command = static_cast<uint8_t>(readRegInstruction),
        .addrMode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE,
        .dummy = 0,
        .dataNbLines = 1,
    };

    return mCommunicationInterface.receive(flashCommand, data, size);
}


bool SFlashConfigurationCommonInterface::waitMemReady()
{
    FlashCommand flashCommand = {
        .command = 0,
    };

    FlashWaitValue flashValue = {
        .match = SR1_WIP_DISABLED << SR1_WIP_IDX,
        .mask = (1 << SR1_WIP_IDX),
        .matchOperator = SFlashCommunicationInterfaceMatchOperator::MATCH_OPERATOR_AND,
        .size = 1,
        .periodMs = 0x10,
        .waitCommand = static_cast<uint8_t>(OpCode::READ_STATUS_REGISTER_RDSR),
        .waitCommandDataNbLines = 1,
    };

    return mCommunicationInterface.transmitAndWaitValue(flashCommand, flashValue);
}

bool SFlashConfigurationCommonInterface::writeEnable()
{
    FlashCommand flashCommand = {
        .command = static_cast<uint8_t>(OpCode::WRITE_ENABLE_WREN),
        .addrMode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE,
        .dummy = 0,
        .dataNbLines = 0,
    };

    FlashWaitValue flashValue = {
        .match = (SR1_WEL_ENABLED << SR1_WEL_IDX),
        .mask = (1 << SR1_WEL_IDX),
        .matchOperator = SFlashCommunicationInterfaceMatchOperator::MATCH_OPERATOR_AND,
        .size = 1,
        .periodMs = 0x10,
        .waitCommand = static_cast<uint8_t>(OpCode::READ_STATUS_REGISTER_RDSR),
        .waitCommandDataNbLines = 1,
    };

    return mCommunicationInterface.transmitAndWaitValue(flashCommand, flashValue);
}
