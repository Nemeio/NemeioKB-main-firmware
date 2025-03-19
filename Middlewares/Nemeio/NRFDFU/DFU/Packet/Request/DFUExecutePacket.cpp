/*
 * DFUPingPacket.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include "DFUExecutePacket.hpp"

DFUExecutePacket::DFUExecutePacket()
{
    createWithPayload(DFUCommand::EXECUTE, array_view<const uint8_t>(nullptr, 0));
}
