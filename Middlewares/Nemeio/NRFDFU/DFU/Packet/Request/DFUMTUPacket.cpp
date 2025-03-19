/*
 * DFUMTUPacket.cpp
 *
 *  Created on: 27 févr. 2021
 *      Author: thomas
 */

#include "DFUMTUPacket.hpp"

DFUMTUPacket::DFUMTUPacket()
{
    createWithPayload(DFUCommand::GET_SERIAL_MTU, array_view<const uint8_t>(nullptr, 0));
}
