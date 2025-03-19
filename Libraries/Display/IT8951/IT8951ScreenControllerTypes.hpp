/*
 * IT8951ScreenControllerTypes.hpp
 *
 *  Created on: 27 oct. 2021
 *      Author: thomas
 */

#ifndef DISPLAY_IT8951_IT8951SCREENCONTROLLERTYPES_HPP_
#define DISPLAY_IT8951_IT8951SCREENCONTROLLERTYPES_HPP_

#include <cstdint>
#include "utils.hpp"

typedef uint16_t IT8951Word;
typedef uint32_t IT8951DWord;

constexpr std::size_t NB_BITS_IT8951_WORD = sizeof(IT8951Word) * BITS_PER_BYTE;

#endif /* DISPLAY_IT8951_IT8951SCREENCONTROLLERTYPES_HPP_ */
