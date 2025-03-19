/*
 * IDFUPacket.hpp
 *
 *  Created on: 18 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_DFU_PACKET_IDFUPACKET_HPP_
#define NEMEIO_NRFDFU_DFU_PACKET_IDFUPACKET_HPP_

#include <cstdint>

enum class DFUCommand : uint8_t
{
    UNKNOWN = 0,
    CREATE_OBJECT = 1,
    SET_PRN = 2,
    CALCULATE_CHECKSUM = 3,
    EXECUTE = 4,
    READ_ERROR = 5,
    SELECT_OBJECT = 6,
    GET_SERIAL_MTU = 7,
    WRITE_OBJECT = 8,
    PING = 9,
    GET_HARDWARE_VERSION = 10,
    GET_FIRMWARE_VERSION = 11,
    RESPONSE = 0x60,
};

class IDFUPacket
{
public:
    virtual ~IDFUPacket() = default;

    /* DFU Packet */
    virtual const uint8_t* data() = 0;
    virtual size_t size() = 0;

    /* DFU Payload */
    virtual DFUCommand getCommand() = 0;
    virtual const uint8_t* payloadData() = 0;
    virtual size_t payloadSize() = 0;
};


#endif /* NEMEIO_NRFDFU_DFU_PACKET_IDFUPACKET_HPP_ */
