/*
 * DFUSetReceipeNotification.hpp
 *
 *  Created on: 27 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_DFU_PACKET_DFUSETRECEIPENOTIFICATIONPACKET_HPP_
#define NEMEIO_NRFDFU_DFU_PACKET_DFUSETRECEIPENOTIFICATIONPACKET_HPP_

#include <cstdint>
#include "DFUPacket.hpp"

class DFUSetReceipeNotificationPacket : public DFUPacket<DFU_PACKET_NORMALIZE_SIZE>
{
public:
    explicit DFUSetReceipeNotificationPacket(uint32_t prn);
    virtual ~DFUSetReceipeNotificationPacket() = default;
};

#endif /* NEMEIO_NRFDFU_DFU_PACKET_DFUSETRECEIPENOTIFICATIONPACKET_HPP_ */
