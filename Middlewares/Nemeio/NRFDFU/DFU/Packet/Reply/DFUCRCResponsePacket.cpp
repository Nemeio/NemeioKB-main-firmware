/*
 * DFUCRCResponsePacket.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include "DFUCRCResponsePacket.hpp"
#include "utils.hpp"

PACK((struct s_dfuPacketCRC
      {
          uint32_t offset;
          uint32_t crc;
      }));

uint32_t DFUCRCResponsePacket::getOffset()
{
    uint32_t offset = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketCRC*>(payloadData());
        offset = content->offset;
    }
    return offset;
}

uint32_t DFUCRCResponsePacket::getCrc()
{
    uint32_t crc = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketCRC*>(payloadData());
        crc = content->crc;
    }
    return crc;
}

bool DFUCRCResponsePacket::isValidPayload()
{
    return isValidResponse() && (payloadSize() == sizeof(s_dfuPacketCRC));
}
