/*
 * strlen.h
 *
 *  Created on: 14 févr. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_TOOLS_STRLEN_H_
#define NEMEIO_TOOLS_STRLEN_H_

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t strnlen(const char* s, size_t maxlen);

#ifdef __cplusplus
}
#endif

#endif /* NEMEIO_TOOLS_STRLEN_H_ */
