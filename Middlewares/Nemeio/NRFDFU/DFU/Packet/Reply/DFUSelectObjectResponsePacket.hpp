/*
 * DFUSelectObjectResponsePacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUSELECTOBJECTRESPONSEPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUSELECTOBJECTRESPONSEPACKET_HPP_

#include <cstdint>
#include "DFUResponsePacket.hpp"

class DFUSelectObjectResponsePacket : public DFUResponsePacket
{
public:
    DFUSelectObjectResponsePacket() = default;
    virtual ~DFUSelectObjectResponsePacket() = default;

    uint32_t getOffset();
    uint32_t getCrc();
    uint32_t getMaxSize();

private:
    bool isValidPayload();
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUSELECTOBJECTRESPONSEPACKET_HPP_ */
