/*
 * DFUWriteObjectPacket.hpp
 *
 *  Created on: 1 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_DFU_PACKET_DFUWRITEOBJECTPACKET_HPP_
#define NEMEIO_NRFDFU_DFU_PACKET_DFUWRITEOBJECTPACKET_HPP_

#include "DFUPacket.hpp"

template<size_t SIZE>
class DFUWriteObjectPacket : public DFUPacket<SIZE>
{
public:
    explicit DFUWriteObjectPacket(array_view<const uint8_t> data);
    virtual ~DFUWriteObjectPacket() = default;
};

template<size_t SIZE>
DFUWriteObjectPacket<SIZE>::DFUWriteObjectPacket(array_view<const uint8_t> data)
{
    this->createWithPayload(DFUCommand::WRITE_OBJECT, data);
}

#endif /* NEMEIO_NRFDFU_DFU_PACKET_DFUWRITEOBJECTPACKET_HPP_ */
