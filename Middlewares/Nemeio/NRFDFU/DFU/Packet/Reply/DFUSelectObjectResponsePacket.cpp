/*
 * DFUSelectObjectResponsePacket.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include "DFUSelectObjectResponsePacket.hpp"
#include "utils.hpp"

#if 0
PACK((struct s_dfuPacketSelectObject {
	uint32_t offset;
	uint32_t crc;
	uint32_t maxSize;
}));
#else
PACK((struct s_dfuPacketSelectObject
      {
          uint32_t maxSize;
          uint32_t offset;
          uint32_t crc;
      }));
#endif

uint32_t DFUSelectObjectResponsePacket::getOffset()
{
    uint32_t offset = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketSelectObject*>(payloadData());
        offset = content->offset;
    }
    return offset;
}

uint32_t DFUSelectObjectResponsePacket::getCrc()
{
    uint32_t crc = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketSelectObject*>(payloadData());
        crc = content->crc;
    }
    return crc;
}

uint32_t DFUSelectObjectResponsePacket::getMaxSize()
{
    uint32_t maxSize = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketSelectObject*>(payloadData());
        maxSize = content->maxSize;
    }
    return maxSize;
}

bool DFUSelectObjectResponsePacket::isValidPayload()
{
    return isValidResponse() && (payloadSize() == sizeof(s_dfuPacketSelectObject));
}
