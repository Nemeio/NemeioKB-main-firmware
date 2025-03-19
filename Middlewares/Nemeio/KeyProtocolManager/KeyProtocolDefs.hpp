/*
 * KeyProtocolDefs.hpp
 *
 *  Created on: Jan 14, 2020
 *      Author: Labo
 */

#ifndef KEYPROTOCOLMANAGER_KEYPROTOCOLDEFS_HPP_
#define KEYPROTOCOLMANAGER_KEYPROTOCOLDEFS_HPP_

#include <stdint.h>

namespace keyprotocoldefs
{
enum class KeyProtocolMode : uint8_t
{
    UNDEFINED = 0,
    HID,
    CUSTOM,
    PAIRING
};
} // namespace keyprotocoldefs

#endif /* KEYPROTOCOLMANAGER_KEYPROTOCOLDEFS_HPP_ */
