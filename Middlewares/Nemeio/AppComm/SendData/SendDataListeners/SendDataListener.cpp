/*
 * SendDataListener.cpp
 *
 *  Created on: Feb 5, 2019
 *      Author: Labo
 */

#include <SendDataListener.hpp>

SendDataListener::SendDataListener(AppCommDefs::SendDataID dataId)
    : StreamDataListener<AppCommDefs::SendDataID>(dataId)
    , mDataLength(0)
{
}

uint8_t SendDataListener::prepareSend(const uint8_t* pHeader, size_t headerLen, uint32_t dataLen)
{
    mDataLength = dataLen;

    return doPrepareSend(pHeader, headerLen, dataLen);
}

uint8_t SendDataListener::sendData(const uint8_t* pData, size_t len)
{
    return doSendData(pData, len);
}

uint8_t SendDataListener::terminateSend()
{
    return doTerminateSend();
}

uint8_t SendDataListener::applySend()
{
    return doApplySend();
}
