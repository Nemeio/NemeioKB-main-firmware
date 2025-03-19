/*
 * DFUGetFirmwareVersionPacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUGETFIRMWAREVERSIONPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUGETFIRMWAREVERSIONPACKET_HPP_

#include "DFUPacket.hpp"

class DFUGetFirmwareVersionPacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    explicit DFUGetFirmwareVersionPacket(uint8_t image);
    virtual ~DFUGetFirmwareVersionPacket() = default;
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUGETFIRMWAREVERSIONPACKET_HPP_ */
