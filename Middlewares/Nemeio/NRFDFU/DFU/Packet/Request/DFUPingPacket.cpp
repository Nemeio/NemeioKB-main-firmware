/*
 * DFUPingPacket.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include "DFUPingPacket.hpp"

DFUPingPacket::DFUPingPacket(uint32_t id)
{
    createWithPayload(DFUCommand::PING,
                      array_view<const uint8_t>(reinterpret_cast<uint8_t*>(&id), sizeof(id)));
}
