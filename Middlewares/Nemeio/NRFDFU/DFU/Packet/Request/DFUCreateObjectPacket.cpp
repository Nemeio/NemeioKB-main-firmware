/*
 * DFUCreateObjectPacket.cpp
 *
 *  Created on: 1 mars 2021
 *      Author: thomas
 */

#include <DFUCreateObjectPacket.hpp>

DFUCreateObjectPacket::DFUCreateObjectPacket(DFUObjectType type, uint32_t size)
{
    struct __attribute__((packed)) dfuObject
    {
        uint8_t type;
        uint32_t size;
    };

    const dfuObject object = {.type = static_cast<uint8_t>(type), .size = size};

    createWithPayload(DFUCommand::CREATE_OBJECT,
                      array_view<const uint8_t>(reinterpret_cast<const uint8_t*>(&object),
                                                sizeof(object)));
}
