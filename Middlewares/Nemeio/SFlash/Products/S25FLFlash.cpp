/*
 * S25FLFlash.cpp
 *
 *  Created on: 7 janv. 2022
 *      Author: thomas
 */

#include <Products/S25FLFlash.hpp>
#include "utils.hpp"
#include "CommonFlashInterface.hpp"

S25FLFlash::S25FLFlash(const FlashInfo& flashInfo,
                       QSPISFlashCommunicationInterface& flashCommunication)
    : Flash(flashInfo,
            flashCommunication,
            flashInfo.mTotalSize <= MAX_VALUE_24_BITS ? FLASH_QSPI_LUT_3BYTES_ADDRESSING
                                                      : FLASH_QSPI_LUT_4BYTES_ADDRESSING)
    , SecureFlashIO(FLASH_SECURE_INFO, flashCommunication, FLASH_QSPI_SECURE_LUT)
{
}

bool S25FLFlash::enterQuadMode()
{
    bool ret = false;
    uint8_t statusConfigRegisters[2] = {0};

    if(!readStatusRegisterCmd(&statusConfigRegisters[0], 1))
    {
        return false;
    }

    SET_BIT(statusConfigRegisters[1], 1 << S25FL_QSPI_STATUS_REGISTER_BIT_INDEX);

    ret = writeRegistersCmd(statusConfigRegisters, sizeof(statusConfigRegisters));

    return ret;
}

bool S25FLFlash::init()
{
    return enterQuadMode();
}

bool S25FLFlash::deinit()
{
    return true;
}
