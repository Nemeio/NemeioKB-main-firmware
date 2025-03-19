/*
 * DFUPacketGenerator.hpp
 *
 *  Created on: 18 févr. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_NRFDFU_COMMANDS_DFURESPONSEPACKET_HPP_
#define NEMEIO_NRFDFU_COMMANDS_DFURESPONSEPACKET_HPP_

#include <cstddef>
#include <array>
#include "array_view.hpp"
#include "IDFUPacket.hpp"

enum class DFUResponseCode : uint8_t
{
    INVALID_CODE = 0,
    SUCCESS = 1,
    NOT_SUPPORTED = 2,
    INVALID_PARAMETER = 3,
    INSUFFICIENT_RESSOURCES = 4,
    INVALID_OBJECT = 5,
    INVALID_SIGNATURE = 6,
    UNSUPPORTED_TYPE = 7,
    OPERATION_NOT_PERMITTED = 8,
    /* Error 9 is not defined */
    OPERATION_FAILED = 10,
    EXTENDED_ERROR = 11,
};

/* TODO Merge with other error for user API */
enum class DFUExtendedResponseCode : uint8_t
{
    NO_ERROR = 0x00,
    INVALID_ERROR_CODE = 0x01,
    WRONG_COMMAND_FORMAT = 0x02,
    UNKNOWN_COMMAND = 0x03,
    INIT_COMMAND_INVALID = 0x04,
    FW_VERSION_FAILURE = 0x05,
    HW_VERSION_FAILURE = 0x06,
    SD_VERSION_FAILURE = 0x07,
    SIGNATURE_MISSING = 0x08,
    WRONG_HASH_TYPE = 0x09,
    HASH_FAILED = 0x0A,
    WRONG_SIGNATURE_TYPE = 0x0B,
    VERIFICATION_FAILED = 0x0C,
    INSUFFICIENT_SPACE = 0x0D,
};

class DFUResponsePacket
{
public:
    DFUResponsePacket();
    explicit DFUResponsePacket(array_view<const uint8_t> content);
    virtual ~DFUResponsePacket() = default;

    /* DFU Packet */
    [[nodiscard]] size_t setData(array_view<const uint8_t> array);
    [[nodiscard]] const uint8_t* data() const;
    [[nodiscard]] size_t size() const;

    /* DFU Payload */
    [[nodiscard]] DFUCommand getCommand();
    [[nodiscard]] const uint8_t* payloadData();
    [[nodiscard]] size_t payloadSize();

    [[nodiscard]] bool isValidResponse();
    [[nodiscard]] DFUResponseCode getResponseCode();
    [[nodiscard]] DFUExtendedResponseCode getExtendedResponseCode();


private:
    static constexpr size_t PROTOCOL_PACKET_MAX_SIZE = 25;
    std::array<uint8_t, PROTOCOL_PACKET_MAX_SIZE> mPacket;
    size_t mPacketSize = 0;

    void reset();
};

#endif /* NEMEIO_NRFDFU_COMMANDS_DFURESPONSEPACKET_HPP_ */
