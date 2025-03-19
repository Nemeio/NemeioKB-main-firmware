/*
 * DFUMTUPacket.hpp
 *
 *  Created on: 27 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_DFU_PACKET_DFUMTUPACKET_HPP_
#define NEMEIO_NRFDFU_DFU_PACKET_DFUMTUPACKET_HPP_

#include "DFUPacket.hpp"

class DFUMTUPacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    DFUMTUPacket();
    virtual ~DFUMTUPacket() = default;
};

#endif /* NEMEIO_NRFDFU_DFU_PACKET_DFUMTUPACKET_HPP_ */
