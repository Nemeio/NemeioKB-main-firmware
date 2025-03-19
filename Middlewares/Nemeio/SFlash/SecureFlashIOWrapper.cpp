/*
 * MX25L25.cpp
 *
 *  Created on: 3 déc. 2021
 *      Author: thomas
 */

#include <SecureFlashIOWrapper.hpp>
#include "utils.hpp"

SecureFlashIOWrapper::SecureFlashIOWrapper(ISecureFlashIO& flash)
    : mSecureFlashIO(flash)
{
}

bool SecureFlashIOWrapper::read(void* buffer, uint32_t addr, size_t size)
{
    return mSecureFlashIO.readSecure(buffer, addr, size);
}

bool SecureFlashIOWrapper::writePage(uint32_t addr, size_t size, uint8_t* data)
{
    return mSecureFlashIO.writePageSecure(addr, size, data);
}

bool SecureFlashIOWrapper::sectorErase(uint32_t addr)
{
    return mSecureFlashIO.sectorEraseSecure(addr);
}

const FlashInfo SecureFlashIOWrapper::getFlashInfo()
{
    return mSecureFlashIO.getFlashInfoSecure();
}
