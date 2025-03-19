/*
 * ReportBuffer.hpp
 *
 *  Created on: Apr 26, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_REPORTBUFFER_HPP_
#define NEMEIO_HID_REPORTBUFFER_HPP_

#include <array>
#include <functional>

template<std::size_t arraySize, uint8_t id>
class ReportBuffer
{
public:
    ReportBuffer();
    ~ReportBuffer() = default;

    void clear();

    void send(std::function<void(const std::array<uint8_t, arraySize>&)> doSend);

    uint8_t operator[](std::size_t i) const;
    uint8_t& operator[](std::size_t i);

    constexpr std::size_t size();

private:
    bool mIsSendNeeded;
    std::array<uint8_t, arraySize> mReportBuffer;

    void init();
};

template<std::size_t arraySize, uint8_t id>
inline ReportBuffer<arraySize, id>::ReportBuffer()
    : mIsSendNeeded(false)
{
    init();
}

template<std::size_t arraySize, uint8_t id>
inline void ReportBuffer<arraySize, id>::clear()
{
    init();
    mIsSendNeeded = true;
}

template<std::size_t arraySize, uint8_t id>
inline void ReportBuffer<arraySize, id>::send(
    std::function<void(const std::array<uint8_t, arraySize>&)> doSend)
{
    if(doSend && mIsSendNeeded)
    {
        doSend(mReportBuffer);
        mIsSendNeeded = false;
    }
}

template<std::size_t arraySize, uint8_t id>
inline uint8_t ReportBuffer<arraySize, id>::operator[](size_t i) const
{
    return mReportBuffer[i];
}

template<std::size_t arraySize, uint8_t id>
inline uint8_t& ReportBuffer<arraySize, id>::operator[](size_t i)
{
    mIsSendNeeded = true;
    return mReportBuffer[i];
}

template<std::size_t arraySize, uint8_t id>
inline constexpr std::size_t ReportBuffer<arraySize, id>::size()
{
    return mReportBuffer.size();
}

template<std::size_t arraySize, uint8_t id>
inline void ReportBuffer<arraySize, id>::init()
{
    mReportBuffer.fill(0);
    mReportBuffer[0] = id;
}

#endif /* NEMEIO_HID_REPORTBUFFER_HPP_ */
