/*
 * DFUPingPacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUPINGPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUPINGPACKET_HPP_

#include "DFUPacket.hpp"

class DFUPingPacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    explicit DFUPingPacket(uint32_t id);
    virtual ~DFUPingPacket() = default;
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUPINGPACKET_HPP_ */
