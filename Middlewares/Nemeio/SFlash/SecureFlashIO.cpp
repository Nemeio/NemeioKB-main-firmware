/*
 * SecureFlash.cpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#include <SecureFlashIO.hpp>

SecureFlashIO::SecureFlashIO(const FlashInfo& flashInfo,
                             ISFlashCommunicationInterface& flashCommunication,
                             const FlashIOLut& flashIoInfos)
    : mFlashConfiguration(flashCommunication)
    , mFlashIO(mFlashConfiguration, flashCommunication, flashIoInfos)
    , mFlashInfo(flashInfo)
{
}

bool SecureFlashIO::readSecure(void* buffer, uint32_t addr, size_t size)
{
    return mFlashIO.read(buffer, addr, size);
}

bool SecureFlashIO::writePageSecure(uint32_t addr, size_t size, uint8_t* data)
{
    return mFlashIO.writePage(addr, size, data);
}

bool SecureFlashIO::sectorEraseSecure(uint32_t addr)
{
    return mFlashIO.sectorErase(addr);
}

const FlashInfo SecureFlashIO::getFlashInfoSecure()
{
    return mFlashInfo;
}
