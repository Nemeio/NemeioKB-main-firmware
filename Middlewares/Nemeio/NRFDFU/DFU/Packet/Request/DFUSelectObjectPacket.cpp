/*
 * DFUSelectObjectPacket.cpp
 *
 *  Created on: 1 mars 2021
 *      Author: thomas
 */

#include "DFUSelectObjectPacket.hpp"

DFUSelectObjectPacket::DFUSelectObjectPacket(DFUObjectType type)
{
    createWithPayload(DFUCommand::SELECT_OBJECT,
                      array_view<const uint8_t>(reinterpret_cast<const uint8_t*>(&type),
                                                sizeof(type)));
}
