/*
 * DFUCreateObjectPacket.hpp
 *
 *  Created on: 1 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_DFU_PACKET_DFUCREATEOBJECTPACKET_HPP_
#define NEMEIO_NRFDFU_DFU_PACKET_DFUCREATEOBJECTPACKET_HPP_

#include "DFUPacket.hpp"

enum class DFUObjectType : uint8_t
{
    FIRMWARE_INVALID = 0,
    FIRMWARE_INIT = 1,
    FIRMWARE_DATA = 2,
};

class DFUCreateObjectPacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    explicit DFUCreateObjectPacket(DFUObjectType type, uint32_t size);
    virtual ~DFUCreateObjectPacket() = default;
};

#endif /* NEMEIO_NRFDFU_DFU_PACKET_DFUCREATEOBJECTPACKET_HPP_ */
