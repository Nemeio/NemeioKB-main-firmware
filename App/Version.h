/*
 * Version.hpp
 *
 *  Created on: Feb 12, 2020
 *      Author: Labo
 */

#ifndef VERSION_HPP_
#define VERSION_HPP_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

const size_t VERSION_MAX_SZ = 16;
const uint16_t APP_MAJOR_NUMBER = APP_MAJOR_NUMBER_DEF;
const uint8_t APP_BUILD_NUMBER = APP_BUILD_NUMBER_DEF;

/* If version match the maximum (0xffff), it's a golden image */
#if defined(APP_GOLDEN_IMAGE) && !(APP_MAJOR_NUMBER_DEF == 2047 && APP_BUILD_NUMBER_DEF == 31)
#error Only maximum version can be golden image
#endif

const uint16_t HARDWARE_REVISION = 1;
const uint16_t PROTOCOL_VERSION = 1;

#ifdef __cplusplus
}
#endif

#endif /* VERSION_HPP_ */
