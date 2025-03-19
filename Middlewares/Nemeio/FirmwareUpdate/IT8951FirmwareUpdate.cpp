/*
 * IT8951ControllerUpgrader.cpp
 *
 *  Created on: 28 juil. 2020
 *      Author: eskoric
 */

#include <IT8951FirmwareUpdate.hpp>
#include "assertTools.h"
#include "utils.hpp"

IT8951FirmwareUpdate::IT8951FirmwareUpdate(IBlockDevice& blockDevice)
    : mBlockDevice(blockDevice)
    , mFlashBuffer(std::bind(&IT8951FirmwareUpdate::_bufferFull,
                             this,
                             std::placeholders::_1,
                             std::placeholders::_2))
{
}

FirmwareUpdateDefs::ErrCode IT8951FirmwareUpdate::initUpdate(uint32_t len)
{
    ASSERT(mBlockDevice.get_program_size() == FIRMWARE_FLASH_WRITE_SIZE);

    if(mbInitialized)
    {
        return FirmwareUpdateDefs::ErrCode::SUCCESS;
    }

    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::SUCCESS;

    const bd_size_t eraseSize = mBlockDevice.get_erase_size()
                                * divRoundUp(len, mBlockDevice.get_erase_size());
    if(!mBlockDevice.is_valid_program(FIRMWARE_FLASH_ADDR, eraseSize))
    {
        ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret && BD_ERROR_OK != mBlockDevice.init())
    {
        ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret
       && mBlockDevice.erase(FIRMWARE_FLASH_ADDR, eraseSize) != BD_ERROR_OK)
    {
        ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret)
    {
        mUpgradeLength = len;
        mbInitialized = true;
        mFlashDest = FIRMWARE_FLASH_ADDR;
    }

    return ret;
}

FirmwareUpdateDefs::ErrCode IT8951FirmwareUpdate::writeFirmware(const uint8_t* pData,
                                                                std::size_t len)
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    if(mbInitialized)
    {
        ret = mFlashBuffer.pushBack(pData, pData + len) == len
                  ? FirmwareUpdateDefs::ErrCode::SUCCESS
                  : FirmwareUpdateDefs::ErrCode::WRITE;
    }

    return ret;
}

FirmwareUpdateDefs::ErrCode IT8951FirmwareUpdate::runUpdate()
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::SUCCESS;

    if(!mbInitialized)
    {
        return FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    if(!mFlashBuffer.flush())
    {
        ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret)
    {
        ret = (mFlashDest - FIRMWARE_FLASH_ADDR >= mUpgradeLength)
                  ? FirmwareUpdateDefs::ErrCode::SUCCESS
                  : FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    mUpgradeLength = 0;
    mbInitialized = false;
    mFlashDest = FIRMWARE_FLASH_ADDR;

    ASSERT(BD_ERROR_OK == mBlockDevice.deinit());

    return ret;
}

std::size_t IT8951FirmwareUpdate::_bufferFull(const uint8_t* data, size_t size)
{
    size_t writtenSize = 0;
    if(mBlockDevice.program(data, mFlashDest, size) == BD_ERROR_OK)
    {
        mFlashDest += size;
        writtenSize = size;
    }

    return writtenSize;
}
