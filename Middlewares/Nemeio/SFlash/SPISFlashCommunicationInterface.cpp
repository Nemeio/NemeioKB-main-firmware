/*
 * SPISFlashCommunicationInterface.cpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#include <SPISFlashCommunicationInterface.hpp>
#include <WriteBuffer.hpp>
#include "CommonFlashInterface.hpp"

SPISFlashCommunicationInterface::SPISFlashCommunicationInterface(iSPI& spi,
                                                                 SingleChipSelector& chipSelector,
                                                                 ISystem& system)
    : mSpi(spi)
    , mChipSelector(chipSelector)
    , mSystem(system)
{
}

bool SPISFlashCommunicationInterface::init(size_t size)
{
    return true;
}

bool SPISFlashCommunicationInterface::deinit()
{
    return true;
}

uint8_t SPISFlashCommunicationInterface::addrModeToSize(SFlashCommunicationInterfaceAddrMode addrMode)
{
    uint8_t addrSize = 3;

    switch(addrMode)
    {
    case SFlashCommunicationInterfaceAddrMode::ADDR_MODE_3_BYTES:
        addrSize = 3;
        break;
    case SFlashCommunicationInterfaceAddrMode::ADDR_MODE_4_BYTES:
        addrSize = 4;
        break;
    default:
        break;
    }

    return addrSize;
}


bool SPISFlashCommunicationInterface::transmit(FlashCommand& command)
{
    ASSERT(command.dummy == 0);

    ISPIErrCode err = mSpi.transmit(&command.command,
                                    sizeof(uint8_t),
                                    mChipSelector,
                                    command.addrMode
                                        == SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE);
    if(ISPI_ERROR_SUCCESS == err
       && command.addrMode != SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
    {
        err = sendAddress(command.address, addrModeToSize(command.addrMode), true);
    }
    else
    {
        endTransfer();
    }

    return (ISPI_ERROR_SUCCESS == err);
}

bool SPISFlashCommunicationInterface::transmit(FlashCommand& command,
                                               uint8_t* txData,
                                               size_t size,
                                               bool bDMA)
{
    ASSERT(command.dummy == 0);

    ISPIErrCode err = mSpi.transmit(&command.command, sizeof(uint8_t), mChipSelector, false);

    if(ISPI_ERROR_SUCCESS == err
       && command.addrMode != SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
    {
        err = sendAddress(command.address, addrModeToSize(command.addrMode), false);
    }

    if(ISPI_ERROR_SUCCESS == err)
    {
        err = mSpi.transmit(txData, size, mChipSelector, true);
    }
    else
    {
        endTransfer();
    }

    return (ISPI_ERROR_SUCCESS == err);
}

bool SPISFlashCommunicationInterface::receive(FlashCommand& command,
                                              uint8_t* rxData,
                                              size_t size,
                                              bool bDMA)
{
    ISPIErrCode err = mSpi.transmit(&command.command, sizeof(uint8_t), mChipSelector, false);

    if(ISPI_ERROR_SUCCESS == err
       && command.addrMode != SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE)
    {
        err = sendAddress(command.address, addrModeToSize(command.addrMode), false);
    }

    for(uint8_t i = 0; i < command.dummy && ISPI_ERROR_SUCCESS == err; ++i)
    {
        uint8_t dummyByte = 0;
        err = mSpi.transmit(&dummyByte, sizeof(dummyByte), mChipSelector, false);
    }

    if(ISPI_ERROR_SUCCESS == err)
    {
        err = mSpi.receive(rxData, size, mChipSelector, true);
    }
    else
    {
        endTransfer();
    }

    return (ISPI_ERROR_SUCCESS == err);
}

bool SPISFlashCommunicationInterface::transmitAndWaitValue(FlashCommand& command,
                                                           FlashWaitValue value)
{
    uint32_t status = 0;
    bool bMatch = false;
    ISPIErrCode err = ISPI_ERROR_SUCCESS;
    uint16_t remainingAttempts = STATUS_REGISTER_POLLING_MAX_ATTEMPT;

    ASSERT(value.size <= sizeof(status));

    if(command.command != 0)
    {
        err = mSpi.transmit(&command.command, sizeof(uint8_t), mChipSelector, true);
    }

    if(ISPI_ERROR_SUCCESS == err)
    {
        err = mSpi.transmit(&value.waitCommand, sizeof(uint8_t), mChipSelector, false);
    }

    while(ISPI_ERROR_SUCCESS == err && !bMatch && remainingAttempts)
    {
        err = mSpi.receive(reinterpret_cast<uint8_t*>(&status), value.size, mChipSelector, false);

        if(ISPI_ERROR_SUCCESS == err)
        {
            if(value.matchOperator == SFlashCommunicationInterfaceMatchOperator::MATCH_OPERATOR_AND)
            {
                bMatch = (status & value.mask) == value.match;
            }
        }

        if(!bMatch)
        {
            mSystem.delay(DelayMs(STATUS_REGISTER_POLLING_DELAY_MS));
        }

        --remainingAttempts;
    }

    if(ISPI_ERROR_SUCCESS == err)
    {
        err = mSpi.receive(reinterpret_cast<uint8_t*>(&status), value.size, mChipSelector, true);
    }
    else
    {
        endTransfer();
    }

    return bMatch;
}

ISPIErrCode SPISFlashCommunicationInterface::sendAddress(uint32_t addr,
                                                         uint8_t addrSize,
                                                         bool bEndTransfer)
{
    static constexpr std::size_t MAX_ADDRESS_SIZE_BYTES = 4;
    WriteBuffer<uint8_t, MAX_ADDRESS_SIZE_BYTES> addressData;

    for(uint8_t i = addrSize; i > 0; --i)
    {
        addressData.pushBack(static_cast<uint8_t>(addr >> ((i - 1) * BITS_PER_BYTE)));
    }

    return mSpi.transmit(addressData.data(), addressData.usedSpace(), mChipSelector, bEndTransfer);
}

void SPISFlashCommunicationInterface::endTransfer()
{
    mSpi.transmit(NULL, 0, mChipSelector, true);
}
