/*
 * FlashWriteBuffer.hpp
 *
 *  Created on: 8 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_SYSTEM_TOOLS_WRITEBUFFER_FLASHWRITEBUFFER_HPP_
#define NEMEIO_SYSTEM_TOOLS_WRITEBUFFER_FLASHWRITEBUFFER_HPP_

#include <algorithm>
#include <functional>
#include <cstddef>
#include "WriteBuffer.hpp"
#include "assertTools.h"

template<typename T, std::size_t BUFFER_SIZE>
class FlashWriteBuffer
{
public:
    FlashWriteBuffer() = delete;
    explicit FlashWriteBuffer(std::function<size_t(const T*, size_t)> bufferFullCallback);
    ~FlashWriteBuffer() = default;

    std::size_t pushBack(const T* first, const T* last);
    bool flush();
    bool partialFlush();
    void reset();
    [[nodiscard]] constexpr std::size_t size() const;

private:
    static constexpr uint8_t DEFAULT_FLASH_VAL = 0xFF;

    WriteBuffer<T, BUFFER_SIZE> mBuffer;
    std::function<size_t(const T*, size_t)> mBufferFullCallback;
};

template<typename T, std::size_t BUFFER_SIZE>
FlashWriteBuffer<T, BUFFER_SIZE>::FlashWriteBuffer(
    std::function<size_t(const T*, size_t)> bufferFullCallback)
    : mBufferFullCallback(bufferFullCallback)
{
}


template<typename T, std::size_t BUFFER_SIZE>
std::size_t FlashWriteBuffer<T, BUFFER_SIZE>::pushBack(const T* first, const T* last)
{
    if(first == nullptr || last == nullptr)
    {
        return 0;
    }

    bool ret = true;
    const T* pData = first;
    const size_t len = std::distance(first, last);
    std::size_t remainingDataLen = len;

    if(!mBuffer.isEmpty())
    {
        const std::size_t copyLen = std::min(remainingDataLen, mBuffer.remainingSpace());
        const std::size_t bufferedLen = mBuffer.pushBack(pData, pData + copyLen);
        ASSERT(bufferedLen == copyLen);

        pData += copyLen;
        remainingDataLen -= copyLen;
    }

    if(mBuffer.isFull())
    {
        ret = flush();
    }

    if(ret && remainingDataLen)
    {
        std::size_t callbackLen = 0;
        while(remainingDataLen >= BUFFER_SIZE)
        {
            callbackLen = mBufferFullCallback(pData, BUFFER_SIZE);
            pData += callbackLen;
            remainingDataLen -= callbackLen;
            if(callbackLen != BUFFER_SIZE)
            {
                break;
            }
        }

        if(remainingDataLen < BUFFER_SIZE)
        {
            const std::size_t bufferedLen = mBuffer.pushBack(pData, pData + remainingDataLen);
            remainingDataLen -= bufferedLen;
        }
    }

    return (len - remainingDataLen);
}


template<typename T, std::size_t BUFFER_SIZE>
bool FlashWriteBuffer<T, BUFFER_SIZE>::flush()
{
    bool ret = true;

    if(!mBuffer.isEmpty())
    {
        const std::size_t callbackLen =
            mBufferFullCallback(mBuffer.data(), BUFFER_SIZE); /* We send the data with 0xFF */
        ret = (callbackLen == BUFFER_SIZE);
    }
    mBuffer.reset(DEFAULT_FLASH_VAL);

    return ret;
}

template<typename T, std::size_t BUFFER_SIZE>
bool FlashWriteBuffer<T, BUFFER_SIZE>::partialFlush()
{
    bool ret = true;

    if(!mBuffer.isEmpty())
    {
        const std::size_t callbackLen = mBufferFullCallback(mBuffer.data(), mBuffer.usedSpace());
        ret = (callbackLen == mBuffer.usedSpace());
    }
    mBuffer.reset(DEFAULT_FLASH_VAL);

    return ret;
}

template<typename T, std::size_t BUFFER_SIZE>
void FlashWriteBuffer<T, BUFFER_SIZE>::reset()
{
    mBuffer.reset(DEFAULT_FLASH_VAL);
}

template<typename T, std::size_t BUFFER_SIZE>
constexpr std::size_t FlashWriteBuffer<T, BUFFER_SIZE>::size() const
{
    return mBuffer.size();
}


#endif /* NEMEIO_SYSTEM_TOOLS_WRITEBUFFER_FLASHWRITEBUFFER_HPP_ */
