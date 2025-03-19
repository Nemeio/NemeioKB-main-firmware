/*
 * Identification.hpp
 *
 *  Created on: Feb 25, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_IDENTIFICATION_IDENTIFICATION_HPP_
#define NEMEIO_IDENTIFICATION_IDENTIFICATION_HPP_

#include <cstdint>

namespace Identification
{
enum class ComponentId : uint8_t
{
    EMBEDDED = 1,
};

enum class ModuleId : uint8_t
{
    UNKNOWN = 0,
    SYSTEM,
    FLASH_MEMORY,
    FILESYSTEM,
    DISPLAY,
    BLE,
    FUELGAUGE,
    BATTERY_CHARGER,
};
}; // namespace Identification

#endif /* NEMEIO_IDENTIFICATION_IDENTIFICATION_HPP_ */
