/*
 * StaticStreamBuffer.hpp
 *
 *  Created on: Mar 23, 2019
 *      Author: Labo
 */

#ifndef TOOLS_STREAMBUFFER_STATICSTREAMBUFFER_HPP_
#define TOOLS_STREAMBUFFER_STATICSTREAMBUFFER_HPP_

#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "SystemTypes.hpp"
#include "Signal.hpp"

class StaticStreamBuffer
{
public:
    StaticStreamBuffer(size_t buffSizeBytes, size_t triggerLevelBytes, uint8_t* pStorageArea);
    virtual ~StaticStreamBuffer();

    StaticStreamBuffer(StaticStreamBuffer&) = delete;
    StaticStreamBuffer& operator=(const StaticStreamBuffer&) = delete;
    StaticStreamBuffer(StaticStreamBuffer&&) = delete;
    StaticStreamBuffer const& operator=(StaticStreamBuffer&&) = delete;

    void startSend();
    size_t send(const uint8_t* pTxBuff, size_t txBuffLen, DelayMs delay);
    size_t receiveNBytes(uint8_t* pRxBuff, size_t rxBuffLen, size_t nbReceiveBytes, DelayMs delay);
    void endSend();

private:
    size_t mBuffSizeBytes;
    StreamBufferHandle_t mStreamBuffer;
    StaticStreamBuffer_t mStreamBufferStruct;

    bool mEndSignal = false;
};

#endif /* TOOLS_STREAMBUFFER_STATICSTREAMBUFFER_HPP_ */
