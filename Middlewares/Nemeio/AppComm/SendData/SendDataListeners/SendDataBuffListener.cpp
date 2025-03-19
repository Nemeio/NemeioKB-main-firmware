/*
 * SendDataBuffListener.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: Labo
 */

#include <assertTools.h>
#include <SendDataBuffListener.hpp>
#include <string.h>
#include <new>

SendDataBuffListener::SendDataBuffListener(AppCommDefs::SendDataID dataId)
    : SendDataListener(dataId)
{
}

uint8_t SendDataBuffListener::doPrepareSend(const uint8_t*, size_t, size_t dataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(mpBuffer != nullptr)
    {
        ret = AppCommDefs::APPCOMMERR_STATE;
    }
    else
    {
        mSizeCompleted = 0;

        mpBuffer.reset(new(std::nothrow) uint8_t[dataLen]);

        if(nullptr == mpBuffer)
        {
            ret = AppCommDefs::APPCOMMERR_NO_RESOURCE;
        }
    }

    return ret;
}

uint8_t SendDataBuffListener::doSendData(const uint8_t* pData, size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    ASSERT(pData != nullptr);

    if(nullptr == mpBuffer)
    {
        return AppCommDefs::APPCOMMERR_STATE;
    }

    if(len + mSizeCompleted <= mDataLength)
    {
        memcpy(mpBuffer.get() + mSizeCompleted, pData, len);
        mSizeCompleted += len;
    }
    else
    {
        ret = AppCommDefs::APPCOMMERR_NO_RESOURCE;
    }
    return ret;
}

uint8_t SendDataBuffListener::doTerminateSend()
{
    return AppCommDefs::APPCOMMERR_SUCCESS;
}

void SendDataBuffListener::doReset()
{
    mSizeCompleted = 0;

    if(mpBuffer != nullptr)
    {
        mpBuffer.reset(nullptr);
    }
}
