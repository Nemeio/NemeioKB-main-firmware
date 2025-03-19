/*
 * StreamObject.hpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMOBJECT_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMOBJECT_HPP_

#include "IStreamDataReader.hpp"
#include "CRTP.hpp"
#include <array>

template<typename T>
class StreamObject : public CRTP<T>
{
public:
    StreamObject() = default;
    virtual ~StreamObject() = default;

    template<std::size_t READ_BUFF_SIZE,
             std::size_t STREAM_BUFF_SIZE,
             int32_t STREAM_TIMEOUT,
             typename InitCallback,
             typename ReadCallback>
    bool read(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
              std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
              std::size_t streamTriggerLevel,
              std::array<uint8_t, READ_BUFF_SIZE>& sourceReadChunkBuffer,
              InitCallback initCb,
              ReadCallback readCb);

protected:
    template<typename InitCallback, typename ReadCallback>
    bool commonRead(IStreamDataReader& reader, InitCallback initCb, ReadCallback cb);
};

template<typename T>
template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         typename InitCallback,
         typename ReadCallback>
bool StreamObject<T>::read(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                           std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                           std::size_t streamTriggerLevel,
                           std::array<uint8_t, READ_BUFF_SIZE>& sourceReadChunkBuffer,
                           InitCallback initCb,
                           ReadCallback readCb)
{
    return this->underlying().template doRead<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>(
        readBuffer, streamBufferStorage, streamTriggerLevel, sourceReadChunkBuffer, initCb, readCb);
}

template<typename T>
template<typename InitCallback, typename ReadCallback>
bool StreamObject<T>::commonRead(IStreamDataReader& reader, InitCallback initCb, ReadCallback readCb)
{
    bool bRet = false;

    ISizeReader& sizeReader = reader;
    if(initCb(sizeReader))
    {
        if(reader.start())
        {
            bRet = readCb(reader);
            bRet &= reader.waitReadFinish();
        }
    }

    return bRet;
}

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMOBJECT_HPP_ */
