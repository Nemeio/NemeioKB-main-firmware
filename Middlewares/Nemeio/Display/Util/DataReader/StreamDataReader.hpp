/*
 * StreamDataReader.hpp
 *
 *  Created on: Oct 21, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMDATAREADER_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMDATAREADER_HPP_

#include <array>
#include <IStreamDataReader.hpp>
#include "StaticStreamBuffer.hpp"
#include "StreamDataReadTask.hpp"
#include "Signal.hpp"
#include "WriteBuffer.hpp"
#include "assertTools.h"
#include "utils.hpp"

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
class StreamDataReader : public IStreamDataReader
{
public:
    StreamDataReader(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                     std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                     std::size_t streamTriggerLevel,
                     StreamDataReadTask& streamDataReadTask);
    virtual ~StreamDataReader() = default;

    const uint8_t* read(size_t size, size_t& retSize);
    bool start();
    bool runRead();
    void notifyReadStart();
    void notifyReadFinish();
    bool waitReadFinish();
    size_t getMaxReadSize();

protected:
    const std::size_t mStreamTriggerLevel;

    void startStream();
    size_t sendToStream(const uint8_t* pTxBuff, size_t txBuffLen, TickType_t delay);
    void endStream();

private:
    std::array<uint8_t, READ_BUFF_SIZE>& mReadBuffer;
    std::array<uint8_t, STREAM_BUFF_SIZE>& mStreamBufferStorage;
    StreamDataReadTask& mStreamDataReadTask;
    StaticStreamBuffer mRxStreamBuff;
    Signal mReadDoneSignal;

    virtual bool doRunRead() = 0;
};

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::StreamDataReader(
    std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
    std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
    std::size_t streamTriggerLevel,
    StreamDataReadTask& streamDataReadTask)
    : mStreamTriggerLevel(streamTriggerLevel)
    , mReadBuffer(readBuffer)
    , mStreamBufferStorage(streamBufferStorage)
    , mStreamDataReadTask(streamDataReadTask)
    , mRxStreamBuff(STREAM_BUFF_SIZE, mStreamTriggerLevel, mStreamBufferStorage.data())
{
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
inline bool StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::waitReadFinish()
{
    return mReadDoneSignal.wait(DelayMs(DelayMs::INFINITE_DELAY));
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
inline bool StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::start()
{
    return mStreamDataReadTask.startRead(*this);
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
inline bool StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::runRead()
{
    return doRunRead();
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
inline void StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::notifyReadStart()
{
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
inline void StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::notifyReadFinish()
{
    mReadDoneSignal.set();
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
inline const uint8_t* StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::read(
    size_t size, size_t& retSize)
{
    if(size > READ_BUFF_SIZE)
    {
        return NULL;
    }

    retSize = mRxStreamBuff.receiveNBytes(mReadBuffer.data(),
                                          READ_BUFF_SIZE,
                                          size,
                                          DelayMs(STREAM_TIMEOUT));

    return retSize != 0 ? mReadBuffer.data() : nullptr;
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
size_t StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::sendToStream(
    const uint8_t* pTxBuff, size_t txBuffLen, TickType_t delay)
{
    return mRxStreamBuff.send(pTxBuff, txBuffLen, delay);
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
size_t StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::getMaxReadSize()
{
    return READ_BUFF_SIZE;
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
void StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::startStream()
{
    mRxStreamBuff.startSend();
}

template<std::size_t READ_BUFF_SIZE, std::size_t STREAM_BUFF_SIZE, int32_t STREAM_TIMEOUT>
void StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>::endStream()
{
    mRxStreamBuff.endSend();
}


#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMDATAREADER_HPP_ */
