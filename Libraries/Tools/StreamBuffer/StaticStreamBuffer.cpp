/*
 * StaticStreamBuffer.cpp
 *
 *  Created on: Mar 23, 2019
 *      Author: Labo
 */

#include "StaticStreamBuffer.hpp"
#include "assertTools.h"
#include <algorithm>
#include "stm32l4xx_hal.h"
#include "FreeRTOSUtils.hpp"

StaticStreamBuffer::StaticStreamBuffer(size_t buffSizeBytes,
                                       size_t triggerLevelBytes,
                                       uint8_t* pStorageArea)
    : mBuffSizeBytes(buffSizeBytes)
{
    mStreamBuffer = xStreamBufferCreateStatic(buffSizeBytes,
                                              triggerLevelBytes,
                                              pStorageArea,
                                              &mStreamBufferStruct);

    ASSERT(mStreamBuffer != NULL);
}

StaticStreamBuffer::~StaticStreamBuffer()
{
    vStreamBufferDelete(mStreamBuffer);
}

size_t StaticStreamBuffer::receiveNBytes(uint8_t* pRxBuff,
                                         size_t rxBuffLen,
                                         size_t nbReceiveBytes,
                                         DelayMs delay)
{
    ASSERT(rxBuffLen >= nbReceiveBytes);

    size_t receivedBytes = 0;
    size_t dataToReceive = nbReceiveBytes;

    TickType_t delayTicks = delayToSystemTicks(delay);

    while(dataToReceive)
    {
        size_t chunkSize = std::min(dataToReceive, mBuffSizeBytes);

        TickType_t tReceiveStart = HAL_GetTick();
        size_t chunkReceivedBytes = xStreamBufferReceive(mStreamBuffer,
                                                         pRxBuff + receivedBytes,
                                                         chunkSize,
                                                         delayTicks);
        TickType_t tReceive = HAL_GetTick() - tReceiveStart;

        if(delayTicks != portMAX_DELAY)
        {
            delayTicks -= std::min(delayTicks, tReceive);
        }

        receivedBytes += chunkReceivedBytes;
        dataToReceive -= chunkReceivedBytes;

        if(0 == delayTicks || (dataToReceive != 0 && mEndSignal))
        {
            break;
        }
    }

    return receivedBytes;
}

size_t StaticStreamBuffer::send(const uint8_t* pTxBuff, size_t txBuffLen, DelayMs delay)
{
    size_t sentBytes = 0;
    size_t dataToSend = txBuffLen;

    TickType_t delayTicks = delayToSystemTicks(delay);

    while(dataToSend)
    {
        size_t chunkSize = std::min(dataToSend, mBuffSizeBytes);

        TickType_t tSendStart = HAL_GetTick();
        size_t chunkSentBytes = xStreamBufferSend(mStreamBuffer,
                                                  pTxBuff + sentBytes,
                                                  chunkSize,
                                                  delayTicks);
        TickType_t tSend = tSendStart - HAL_GetTick();

        if(delayTicks != portMAX_DELAY)
        {
            delayTicks -= std::min(delayTicks, tSend);
        }

        sentBytes += chunkSentBytes;
        dataToSend -= chunkSentBytes;

        if(0 == delayTicks)
        {
            break;
        }
    }

    return sentBytes;
}

void StaticStreamBuffer::startSend()
{
    mEndSignal = false;
}

void StaticStreamBuffer::endSend()
{
    mEndSignal = true;
}
