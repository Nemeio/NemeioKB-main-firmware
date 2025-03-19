/*
 * DFUSelectObjectPacket.hpp
 *
 *  Created on: 1 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_DFU_PACKET_DFUSELECTOBJECTPACKET_HPP_
#define NEMEIO_NRFDFU_DFU_PACKET_DFUSELECTOBJECTPACKET_HPP_

#include "DFUPacket.hpp"
#include "DFUCreateObjectPacket.hpp" //FIX for enum

class DFUSelectObjectPacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    explicit DFUSelectObjectPacket(DFUObjectType type);
    virtual ~DFUSelectObjectPacket() = default;
};

#endif /* NEMEIO_NRFDFU_DFU_PACKET_DFUSELECTOBJECTPACKET_HPP_ */
