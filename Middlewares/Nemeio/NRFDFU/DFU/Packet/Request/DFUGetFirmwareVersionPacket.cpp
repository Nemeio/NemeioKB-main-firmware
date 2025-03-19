/*
 * DFUGetFirmwareVersionPacket.cpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#include "DFUGetFirmwareVersionPacket.hpp"

DFUGetFirmwareVersionPacket::DFUGetFirmwareVersionPacket(uint8_t image)
{
    createWithPayload(DFUCommand::GET_FIRMWARE_VERSION,
                      array_view<const uint8_t>(&image, sizeof(image)));
}
