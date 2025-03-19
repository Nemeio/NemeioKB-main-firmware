/*
 * AbstractNBufferLogAdapter.hpp
 *
 *  Created on: May 23, 2019
 *      Author: Labo
 */

#ifndef LOG_LOGADAPTER_ABSTRACTNBUFFERLOGADAPTER_HPP_
#define LOG_LOGADAPTER_ABSTRACTNBUFFERLOGADAPTER_HPP_

#include "AutoLock.h"
#include "AbstractLogAdapter.hpp"
#include "WriteBuffer.hpp"
#include "assertTools.h"


template<std::size_t LogBufferSize>
class AbstractNBufferLogAdapter : public AbstractLogAdapter
{
public:
    AbstractNBufferLogAdapter() = default;
    ~AbstractNBufferLogAdapter() override = default;

    LogErrCode flush() final;

private:
    WriteBuffer<uint8_t, LogBufferSize> mLogWriteBuffer;
    Mutex mLock;

    std::size_t writeBuffer(const uint8_t* pData, std::size_t len) final;

    virtual LogErrCode writeLog(const uint8_t* pData, std::size_t len) = 0;
};

template<std::size_t LogBufferSize>
inline LogErrCode AbstractNBufferLogAdapter<LogBufferSize>::flush()
{
    AutoLock autoLock(mLock, osWaitForever);

    LogErrCode res = writeLog(mLogWriteBuffer.data(), mLogWriteBuffer.usedSpace());
    mLogWriteBuffer.reset();

    return res;
}

template<std::size_t LogBufferSize>
inline std::size_t AbstractNBufferLogAdapter<LogBufferSize>::writeBuffer(const uint8_t* pData,
                                                                         std::size_t len)
{
    AutoLock autoLock(mLock, osWaitForever);

    return mLogWriteBuffer.pushBack(pData, pData + len);
}

#endif /* LOG_LOGADAPTER_ABSTRACTNBUFFERLOGADAPTER_HPP_ */
