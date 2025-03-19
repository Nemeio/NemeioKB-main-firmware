/*
 * DFUPacketGenererator.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include "DFUResponsePacket.hpp"
#include <cstring>
#include "utils.hpp"

PACK((struct s_dfuPacketResponse
      {
          DFUCommand response;
          DFUCommand command;
          DFUResponseCode code;
          uint8_t payload[];
      }));

static constexpr size_t headerSize = sizeof(s_dfuPacketResponse);

void DFUResponsePacket::reset()
{
    mPacket.fill(0);
    mPacketSize = 0;
}

DFUResponsePacket::DFUResponsePacket()
{
    reset();
}

DFUResponsePacket::DFUResponsePacket(array_view<const uint8_t> array)
{
    reset();
    if(array.size() <= mPacket.size())
    {
        mPacketSize = array.size();
        memcpy(mPacket.data(), array.data(), mPacketSize);
    }
}

const uint8_t* DFUResponsePacket::data() const
{
    return mPacket.data();
}

size_t DFUResponsePacket::size() const
{
    return mPacketSize;
}

DFUCommand DFUResponsePacket::getCommand()
{
    if(!isValidResponse())
    {
        return DFUCommand::UNKNOWN;
    }

    auto response = reinterpret_cast<s_dfuPacketResponse*>(mPacket.data());
    return response->command;
}

const uint8_t* DFUResponsePacket::payloadData()
{
    uint8_t* payload = nullptr;

    if(payloadSize() > 0)
    {
        payload = reinterpret_cast<s_dfuPacketResponse*>(mPacket.data())->payload;
    }

    return payload;
}

size_t DFUResponsePacket::payloadSize()
{
    if(!isValidResponse())
    {
        return 0;
    }

    return mPacketSize - headerSize;
}

bool DFUResponsePacket::isValidResponse()
{
    if(mPacketSize < headerSize)
    {
        return false;
    }

    return (static_cast<DFUCommand>(mPacket[0]) == DFUCommand::RESPONSE);
}

DFUResponseCode DFUResponsePacket::getResponseCode()
{
    if(!isValidResponse())
    {
        return DFUResponseCode::INVALID_CODE;
    }
    auto responsePacket = reinterpret_cast<s_dfuPacketResponse*>(mPacket.data());

    return responsePacket->code;
}

DFUExtendedResponseCode DFUResponsePacket::getExtendedResponseCode()
{
    DFUExtendedResponseCode code = DFUExtendedResponseCode::NO_ERROR;
    if(!isValidResponse())
    {
        return DFUExtendedResponseCode::NO_ERROR;
    }
    auto responsePacket = reinterpret_cast<s_dfuPacketResponse*>(mPacket.data());

    if(responsePacket->code == DFUResponseCode::EXTENDED_ERROR)
    {
        code = static_cast<DFUExtendedResponseCode>(responsePacket->payload[0]);
    }

    return code;
}

size_t DFUResponsePacket::setData(array_view<const uint8_t> array)
{
    if(mPacket.size() >= array.size())
    {
        mPacketSize = array.size();
        memcpy(mPacket.data(), array.data(), mPacketSize);
    }
    else
    {
        mPacketSize = 0;
    }

    return mPacketSize;
}
