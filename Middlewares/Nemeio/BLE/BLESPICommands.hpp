/*
 * BLESPICommands.hpp
 *
 *  Created on: Sep 30, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_BLE_BLESPICOMMANDS_HPP_
#define NEMEIO_BLE_BLESPICOMMANDS_HPP_

#include <cstdint>
#include <cstddef>

namespace BLESPICommands
{
enum class CommandId : uint8_t
{
    INVALID_CMD = 0,
    UPDATE_REPORT_DESC = 1,
    SEND_INPUT_REPORT = 2,
    SET_HIBERNATION = 3,
    SET_ADVERTISING = 4,
    CONFIGURE_ADVERTISING = 5,
    SET_RADIO_POWER = 6,
    DELETE_WHITELIST = 7,
    DELETE_BONDS = 8,
    GET_DEVICE_COUNT = 9,
    GET_DEVICE_INFO = 10,
    DELETE_DEVICE = 11,
    START_DFU = 12,
    DFU_DATA = 13,
    SERIAL_DATA = 14,
    GET_FW_VERSION = 15,
    NUMERIC_COMPARISON = 16,
    PAIRING_ABORT = 17,
    OUTPUT_REPORT = 18,
    BLE_EVENT = 19,
    GET_PROTOCOL_VERSION = 20,
    FACTORY_RESET = 21,
    SET_MAC_ADDRESS = 22,
    SET_RADIO_MODE = 23,
    SET_DEVICE_NAME = 24,
    SET_DEVICE_PROPERTIES = 25,
    DFU_COMMIT_VERSION = 26, /* Validate application version to avoid rollback */
    NRF_STARTED = 27,
    ENTERED_DFU = 28,
};

enum class CmdRetCode : uint8_t
{
    SUCCESS = 0,
    UNEXPECTED,
};

inline constexpr size_t MAX_COMMAND_LENGTH = 255;
inline constexpr size_t MAX_PARAMETERS_SIZE = 141;
} // namespace BLESPICommands


#endif /* NEMEIO_BLE_BLESPICOMMANDS_HPP_ */
