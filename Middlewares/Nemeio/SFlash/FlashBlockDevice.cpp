/*
 * QSPIFlashBlockDevice.cpp
 *
 *  Created on: 3 déc. 2021
 *      Author: thomas
 */

#include <FlashBlockDevice.hpp>
#include "utils.hpp"
#include "AutoLock.h"

FlashBlockDevice::FlashBlockDevice(IFlashIO& flash)
    : mFlash(flash)
{
}

int FlashBlockDevice::init()
{
    return BD_ERROR_OK;
}

int FlashBlockDevice::deinit()
{
    return BD_ERROR_OK;
}

int FlashBlockDevice::read(void* buffer, bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_read(addr, size));

    AutoLock lock(mMutex, DelayMs(MUTEX_FLASH_OPERATION_TIMEOUT_MS));

    return (mFlash.read(buffer, addr, size) ? BD_ERROR_OK : BD_ERROR_DEVICE_ERROR);
}

int FlashBlockDevice::program(const void* buffer, bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_program(addr, size));

    AutoLock lock(mMutex, DelayMs(MUTEX_FLASH_OPERATION_TIMEOUT_MS));

    uint8_t* data = (uint8_t*) buffer;
    int ret = 0;

    while(size > 0)
    {
        ret = (mFlash.writePage(addr, std::min(size, get_program_size()), data)
                   ? BD_ERROR_OK
                   : BD_ERROR_DEVICE_ERROR);
        if(ret != BD_ERROR_OK)
        {
            break;
        }
        size -= std::min(size, get_program_size());
        addr += get_program_size();
        data += get_program_size();
    }

    return ret;
}

int FlashBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_erase(addr, size));

    AutoLock lock(mMutex, DelayMs(MUTEX_FLASH_OPERATION_TIMEOUT_MS));

    int ret = BD_ERROR_OK;
    while(size > 0)
    {
        ret = (mFlash.sectorErase(addr) ? BD_ERROR_OK : BD_ERROR_DEVICE_ERROR);
        if(ret != BD_ERROR_OK)
        {
            break;
        }
        addr += get_erase_size();
        size -= get_erase_size();
    }

    return ret;
}

bd_size_t FlashBlockDevice::get_read_size() const
{
    return mFlash.getFlashInfo().mReadSize;
}

bd_size_t FlashBlockDevice::get_program_size() const
{
    return mFlash.getFlashInfo().mPageSize;
}

bd_size_t FlashBlockDevice::get_erase_size() const
{
    return mFlash.getFlashInfo().mEraseSize;
}

bd_size_t FlashBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return get_erase_size();
}

bd_size_t FlashBlockDevice::size() const
{
    return mFlash.getFlashInfo().mTotalSize;
}

void FlashBlockDevice::lock()
{
}

void FlashBlockDevice::unlock()
{
}
