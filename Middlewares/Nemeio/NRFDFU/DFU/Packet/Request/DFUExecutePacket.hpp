/*
 * DFUExecutePacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUEXECUTEPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUEXECUTEPACKET_HPP_

#include "DFUPacket.hpp"

class DFUExecutePacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    DFUExecutePacket();
    virtual ~DFUExecutePacket() = default;
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUEXECUTEPACKET_HPP_ */
