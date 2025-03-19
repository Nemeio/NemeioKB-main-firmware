/*
 * systemUtils.h
 *
 *  Created on: May 28, 2019
 *      Author: Labo
 */

#ifndef TOOLS_SYSTEMUTILS_H_
#define TOOLS_SYSTEMUTILS_H_

#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include "FreeRTOS.h"

void SystemReset();

inline void SystemReset()
{
    HAL_NVIC_SystemReset();

    while(1)
        ;
}

static inline bool isInISRContext()
{
    return pdTRUE == xPortIsInsideInterrupt();
}

#endif /* TOOLS_SYSTEMUTILS_H_ */
