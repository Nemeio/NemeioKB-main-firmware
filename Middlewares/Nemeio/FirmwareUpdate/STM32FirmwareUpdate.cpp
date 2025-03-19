/*
 * STM32FirmwareUpdate.cpp
 *
 *  Created on: May 17, 2019
 *      Author: Labo
 */

#include "se_def.h"
#include "assertTools.h"
#include <string.h>
#include "STM32FirmwareUpdate.hpp"

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdate::initUpdate(uint32_t len)
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;

    SFU_FwImageFlashTypeDef fwImageDlArea;
    memset(&fwImageDlArea, 0, sizeof(fwImageDlArea));

    mUpgradeLength = 0;
    resetFlashAddr();

    if(mSFU.getDownloadAreaInfo(&fwImageDlArea))
    {
        if(len > fwImageDlArea.MaxSizeInBytes)
        {
            ret = FirmwareUpdateDefs::ErrCode::NOMEM;
        }
        else
        {
            if(mFlashInterface.eraseSize(reinterpret_cast<void*>(fwImageDlArea.ImageAddr),
                                         fwImageDlArea.MaxSizeInBytes))
            {
                mFlashDest = fwImageDlArea.ImageAddr;
                mFlashDestBegin = mFlashDest;
                mUpgradeLength = len;
                ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
            }
        }
    }

    if(FirmwareUpdateDefs::ErrCode::SUCCESS != ret)
    {
        resetFlashAddr();
    }

    return ret;
}

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdate::writeFirmware(const uint8_t* pData, std::size_t len)
{
    if(nullptr == mFlashDest)
    {
        return FirmwareUpdateDefs::ErrCode::STATE;
    }

    return mFlashBuffer.pushBack(pData, pData + len) == len ? FirmwareUpdateDefs::ErrCode::SUCCESS
                                                            : FirmwareUpdateDefs::ErrCode::WRITE;
}

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdate::runUpdate()
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::SUCCESS;

    if(nullptr == mFlashDestBegin)
    {
        return FirmwareUpdateDefs::ErrCode::STATE;
    }

    if(!mFlashBuffer.flush())
    {
        ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;
    }

    resetFlashAddr();

    return ret;
}

STM32FirmwareUpdate::STM32FirmwareUpdate(IFlashInterface& flashInterface, ISFU& sfu)
    : mFlashInterface(flashInterface)
    , mSFU(sfu)
    , mFlashBuffer(std::bind(&STM32FirmwareUpdate::_bufferFull,
                             this,
                             std::placeholders::_1,
                             std::placeholders::_2))
{
    resetFlashAddr();
}

void STM32FirmwareUpdate::resetFlashAddr()
{
    mFlashDest = nullptr;
    mFlashDestBegin = nullptr;
}

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdate::writeFlash(const uint8_t* pSrc, std::size_t len)
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::WRITE;

    ASSERT(mFlashDest != nullptr);
    ASSERT(pSrc != nullptr);

    if(mFlashInterface.write(mFlashDest, pSrc, len))
    {
        mFlashDest += len;
        ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
    }

    return ret;
}

std::size_t STM32FirmwareUpdate::_bufferFull(const uint8_t* data, size_t size)
{
    return (writeFlash(data, size) == FirmwareUpdateDefs::ErrCode::SUCCESS ? size : 0);
}
