/*
 * BLEDefs.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_BLE_BLEDEFS_HPP_
#define NEMEIO_BLE_BLEDEFS_HPP_

#include <array>
#include <cstdint>
#include "assertTools.h"
#include "utils.hpp"

namespace BLEDefs
{
inline constexpr uint32_t TIMEOUT_SECONDS = 30000;
inline constexpr std::size_t PASSKEY_SIZE = 6;

struct Passkey
{
    std::array<char, PASSKEY_SIZE> value;

    template<typename InputIterator>
    Passkey(InputIterator passkeyBegin, InputIterator passkeyEnd)
    {
        ASSERT(passkeyEnd - passkeyBegin == PASSKEY_SIZE);
        std::copy(passkeyBegin, passkeyEnd, value.begin());
    }
    Passkey() { value.fill('\0'); }
};

enum class ErrCode
{
    SUCCESS,
    TRANSFER_FAILED,
    INVALID_PARAMETER,
    RESP_TIMEOUT,
    DEVICE_NOT_READY,
    BUSY,
    UNKNOWN
};

typedef uint8_t ProtocolVersion;
inline constexpr ProtocolVersion PROTOCOL_VERSION = 1;

enum class TestMode : uint8_t
{
    DISABLED = 0,
    TX_LOWEST_CHANNEL,
    TX_MIDDLE_CHANNEL,
    TX_HIGHEST_CHANNEL,
    RX,
    APP_HID_DATA,
    UNKNOWN
};

PACK((struct FirmwareVersion
      {
          uint16_t major;
          uint8_t minor;
      }));

inline constexpr size_t DEVICE_NAME_MAX_SIZE = 32;

struct bleDeviceProperties
{
    char software_revision[10];
};

} // namespace BLEDefs

#endif /* NEMEIO_BLE_BLEDEFS_HPP_ */
