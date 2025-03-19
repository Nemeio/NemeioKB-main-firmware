/*
 * S25FLFlash.cpp
 *
 *  Created on: 7 janv. 2022
 *      Author: thomas
 */

#include <Products/MX25LFlash.hpp>
#include "utils.hpp"
#include "CommonFlashInterface.hpp"

MX25LFlash::MX25LFlash(const FlashInfo& flashInfo,
                       QSPISFlashCommunicationInterface& flashCommunication)
    : Flash(flashInfo,
            flashCommunication,
            flashInfo.mTotalSize <= MAX_VALUE_24_BITS ? FLASH_QSPI_LUT_3BYTES_ADDRESSING
                                                      : FLASH_QSPI_LUT_4BYTES_ADDRESSING)
    , SecureFlashIO(FLASH_SECURE_INFO, flashCommunication, FLASH_QSPI_LUT_3BYTES_ADDRESSING)
    , mFlashComInterface(flashCommunication)
    , mQuadMode(true)
{
}

MX25LFlash::MX25LFlash(const FlashInfo& flashInfo,
                       SPISFlashCommunicationInterface& flashCommunication)
    : Flash(flashInfo, flashCommunication, FLASH_SPI_LUT)
    , SecureFlashIO(FLASH_SECURE_INFO, flashCommunication, FLASH_SPI_LUT)
    , mFlashComInterface(flashCommunication)
{
    ASSERT(flashInfo.mTotalSize <= MAX_VALUE_24_BITS); // Not supported on SPI yet
}

bool MX25LFlash::enterQuadMode()
{
    bool ret = false;
    uint8_t statusConfigRegisters[2] = {0};

    if(!readStatusRegisterCmd(&statusConfigRegisters[0], 1))
    {
        return false;
    }

    SET_BIT(statusConfigRegisters[0], 1 << MX25L_QSPI_STATUS_REGISTER_BIT_INDEX);

    ret = writeRegistersCmd(statusConfigRegisters, sizeof(statusConfigRegisters));

    return ret;
}

bool MX25LFlash::init()
{
    bool ret = true;

    if(mQuadMode)
    {
        ret = enterQuadMode();
    }

    return ret;
}

bool MX25LFlash::deinit()
{
    return true;
}

bool MX25LFlash::readSecure(void* buffer, uint32_t addr, size_t size)
{
    bool ret = enterSecureMode();

    if(ret)
    {
        SecureFlashIO::readSecure(buffer, addr, size);
    }

    exitSecureMode();

    return ret;
}

bool MX25LFlash::writePageSecure(uint32_t addr, size_t size, uint8_t* data)
{
    bool ret = enterSecureMode();

    if(ret)
    {
        SecureFlashIO::writePageSecure(addr, size, data);
    }

    exitSecureMode();

    return ret;
}

bool MX25LFlash::sectorEraseSecure(uint32_t addr)
{
    /* We can't set bit from 0 to 1 */
    return false;
}

bool MX25LFlash::enterSecureMode()
{
    FlashCommand command = {
        .command = static_cast<uint8_t>(MX25LOpCode::ENTER_SECURITY_MODE),
        .addrMode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE,
        .dummy = 0,
    };

    return mFlashComInterface.transmit(command);
}

bool MX25LFlash::exitSecureMode()
{
    FlashCommand command = {
        .command = static_cast<uint8_t>(MX25LOpCode::EXIT_SECURITY_MODE),
        .addrMode = SFlashCommunicationInterfaceAddrMode::ADDR_MODE_NONE,
        .dummy = 0,
    };

    return mFlashComInterface.transmit(command);
}
