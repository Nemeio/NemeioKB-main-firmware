/*
 * DummyFlash.cpp
 *
 *  Created on: 6 déc. 2021
 *      Author: thomas
 */

#include <DummyFlash.hpp>
#include "utils.hpp"

bool DummyFlash::init()
{
    return 0;
}

bool DummyFlash::deinit()
{
    return 0;
}

bool DummyFlash::read(void* buffer, uint32_t addr, size_t size)
{
    return -1;
}

bool DummyFlash::writePage(uint32_t addr, size_t size, uint8_t* data)
{
    return -1;
}

bool DummyFlash::sectorErase(uint32_t addr)
{
    return -1;
}

const FlashInfo DummyFlash::getFlashInfo()
{
    return FlashInfo{1, 1, 256, MO_TO_BYTES(32)}; /* ASSERT if use of 0 */
}

bool DummyFlash::readStatusRegisterCmd(uint8_t* data, size_t size)
{
    return false;
}

bool DummyFlash::readConfigurationRegisterCmd(uint8_t* data, size_t size)
{
    return false;
}

bool DummyFlash::writeRegistersCmd(uint8_t* data, size_t size)
{
    return false;
}

uint64_t DummyFlash::getUniqueId()
{
    return 0;
}

uint32_t DummyFlash::getIdentifier()
{
    return 0;
}

bool DummyFlash::waitMemReady()
{
    return false;
}

bool DummyFlash::writeEnable()
{
    return false;
}
