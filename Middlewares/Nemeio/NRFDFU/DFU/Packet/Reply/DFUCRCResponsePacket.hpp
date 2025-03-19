/*
 * DFUCRCResponsePacket.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFUCRCRESPONSEPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFUCRCRESPONSEPACKET_HPP_

#include <cstdint>
#include "DFUResponsePacket.hpp"

class DFUCRCResponsePacket : public DFUResponsePacket
{
public:
    DFUCRCResponsePacket() = default;
    virtual ~DFUCRCResponsePacket() = default;

    uint32_t getOffset();
    uint32_t getCrc();

private:
    bool isValidPayload();
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFUSELECTOBJECTRESPONSEPACKET_HPP_ */
