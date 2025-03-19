/*
 * DFUSetReceipeNotification.cpp
 *
 *  Created on: 27 févr. 2021
 *      Author: thomas
 */

#include "DFUSetReceipeNotificationPacket.hpp"

/* prn is the number of write before NRF send a CRC response (every prn times)). 0 to never */
DFUSetReceipeNotificationPacket::DFUSetReceipeNotificationPacket(uint32_t prn)
{
    createWithPayload(DFUCommand::SET_PRN,
                      array_view<const uint8_t>(reinterpret_cast<uint8_t*>(&prn), sizeof(prn)));
}
