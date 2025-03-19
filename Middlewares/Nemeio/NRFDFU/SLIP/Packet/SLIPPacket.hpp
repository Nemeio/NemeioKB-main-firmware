/*
 * SLIPPacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_SLIPPROTOCOL_HPP_
#define NEMEIO_NRFDFU_SLIPPROTOCOL_HPP_

#include <cstddef>
#include <cstdint>
#include <array>
#include <cstring>
#include <sys/types.h>
#include "array_view.hpp"

template<size_t SIZE>
class SLIPPacket
{
public:
    SLIPPacket();
    SLIPPacket(array_view<const uint8_t> data);
    ssize_t createWithPayload(array_view<const uint8_t> payload);

    /* SLIP Packet */
    const uint8_t* data() const;
    size_t size() const;

    /* SLIP Payload */
    ssize_t getPayload(array_view<uint8_t> payload);

private:
    static constexpr uint8_t SLIP_BYTE_END = 0xC0;
    static constexpr uint8_t SLIP_BYTE_ESC = 0xDB;
    static constexpr uint8_t SLIP_BYTE_ESC_END = 0xDC;
    static constexpr uint8_t SLIP_BYTE_ESC_ESC = 0xDD;

    std::array<uint8_t, SIZE> mPacket;
    size_t mPacketSize = 0;

    ssize_t encode(array_view<const uint8_t> payload);
    ssize_t decode(array_view<uint8_t> payload);
};

template<size_t SIZE>
SLIPPacket<SIZE>::SLIPPacket()
{
    mPacketSize = 0;
}

template<size_t SIZE>
SLIPPacket<SIZE>::SLIPPacket(array_view<const uint8_t> data)
{
    mPacketSize = 0;
    if(mPacket.size() >= data.size())
    {
        mPacketSize = data.size();
        memcpy(mPacket.data(), data.data(), mPacketSize);
    }
}

template<size_t SIZE>
ssize_t SLIPPacket<SIZE>::createWithPayload(array_view<const uint8_t> payload)
{
    return encode(payload);
}

template<size_t SIZE>
ssize_t SLIPPacket<SIZE>::getPayload(array_view<uint8_t> payload)
{
    return decode(payload);
}

template<size_t SIZE>
ssize_t SLIPPacket<SIZE>::decode(array_view<uint8_t> payload)
{
    if(mPacket.size() <= 0 && mPacket[mPacket.size() - 1] != SLIP_BYTE_END)
    {
        return -1;
    }

    uint16_t currentPacketPos = 0;
    ssize_t payloadSize = 0;
    bool escape = false;
    bool validPacket = false;
    while(currentPacketPos < payload.size())
    {
        if(escape)
        {
            escape = false;
            if(mPacket[currentPacketPos] == SLIP_BYTE_ESC_END)
            {
                payload[payloadSize++] = SLIP_BYTE_END;
            }
            else if(mPacket[currentPacketPos] == SLIP_BYTE_ESC_ESC)
            {
                payload[payloadSize++] = SLIP_BYTE_ESC;
            }
            else
            {
                break;
            }
        }
        else if(mPacket[currentPacketPos] == SLIP_BYTE_ESC)
        {
            escape = true;
        }
        else if(mPacket[currentPacketPos] == SLIP_BYTE_END)
        {
            validPacket = true;
            break;
        }
        else
        {
            payload[payloadSize++] = mPacket[currentPacketPos];
        }
        currentPacketPos++;
    }

    if(!validPacket)
    {
        payloadSize = 0;
    }

    return payloadSize;
}

template<size_t SIZE>
ssize_t SLIPPacket<SIZE>::encode(array_view<const uint8_t> payload)
{
    ssize_t ret = 0;
    mPacketSize = 0;
    mPacket.fill(0);
    for(auto& byte: payload)
    {
        if(mPacketSize > mPacket.size())
        {
            ret = -1;
            break;
        }
        switch(byte)
        {
        case SLIP_BYTE_END:
            mPacket[mPacketSize++] = SLIP_BYTE_ESC;
            mPacket[mPacketSize++] = SLIP_BYTE_ESC_END;
            break;
        case SLIP_BYTE_ESC:
            mPacket[mPacketSize++] = SLIP_BYTE_ESC;
            mPacket[mPacketSize++] = SLIP_BYTE_ESC_ESC;
            break;
        default:
            mPacket[mPacketSize++] = byte;
            break;
        }
    }

    if(ret == 0)
    {
        mPacket[mPacketSize++] = SLIP_BYTE_END;
        ret = mPacketSize;
    }
    else
    {
        mPacketSize = 0;
    }

    return ret;
}

/* SLIP Packet */
template<size_t SIZE>
const uint8_t* SLIPPacket<SIZE>::data() const
{
    return mPacket.data();
}

template<size_t SIZE>
size_t SLIPPacket<SIZE>::size() const
{
    return mPacketSize;
}

#endif /* NEMEIO_NRFDFU_SLIPPROTOCOL_HPP_ */
