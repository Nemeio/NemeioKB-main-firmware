/*
 * DFUFirmwareVersionResponsePacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUFIRMWAREVERSIONRESPONSEPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUFIRMWAREVERSIONRESPONSEPACKET_HPP_

#include <cstdint>
#include "DFUResponsePacket.hpp"

class DFUFirmwareVersionResponsePacket : public DFUResponsePacket
{
public:
    DFUFirmwareVersionResponsePacket() = default;
    virtual ~DFUFirmwareVersionResponsePacket() = default;

    [[nodiscard]] uint8_t getType();
    [[nodiscard]] uint32_t getVersion();
    [[nodiscard]] uint32_t getSize();
    [[nodiscard]] uint32_t getStartAddress();

private:
    bool isValidPayload();
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUFIRMWAREVERSIONRESPONSEPACKET_HPP_ */
