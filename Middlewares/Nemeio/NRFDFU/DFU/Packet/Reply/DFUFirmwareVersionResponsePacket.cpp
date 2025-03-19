/*
 * DFUFirmwareVersionResponsePacket.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include <cstdint>
#include "DFUFirmwareVersionResponsePacket.hpp"
#include "utils.hpp"

PACK((struct s_dfuPacketFirmwareVersion
      {
          uint8_t type;
          uint32_t version;
          uint32_t startAddress;
          uint32_t size;
      }));

uint8_t DFUFirmwareVersionResponsePacket::getType()
{
    uint8_t type = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketFirmwareVersion*>(payloadData());
        type = content->type;
    }
    return type;
}

uint32_t DFUFirmwareVersionResponsePacket::getVersion()
{
    uint32_t version = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketFirmwareVersion*>(payloadData());
        version = content->version;
    }
    return version;
}

uint32_t DFUFirmwareVersionResponsePacket::getStartAddress()
{
    uint32_t startAddress = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketFirmwareVersion*>(payloadData());
        startAddress = content->startAddress;
    }
    return startAddress;
}

uint32_t DFUFirmwareVersionResponsePacket::getSize()
{
    uint32_t size = 0;
    if(isValidPayload())
    {
        auto content = reinterpret_cast<const s_dfuPacketFirmwareVersion*>(payloadData());
        size = content->size;
    }
    return size;
}

bool DFUFirmwareVersionResponsePacket::isValidPayload()
{
    return isValidResponse() && (payloadSize() == sizeof(s_dfuPacketFirmwareVersion));
}
