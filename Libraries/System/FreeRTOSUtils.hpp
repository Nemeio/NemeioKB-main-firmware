/*
 * FreeRTOSUtils.h
 *
 *  Created on: Jan 24, 2019
 *      Author: Labo
 */

#ifndef FREERTOSUTILS_HPP_
#define FREERTOSUTILS_HPP_

#include "utils.hpp"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "SystemTypes.hpp"

constexpr TickType_t delayToSystemTicks(DelayMs delay)
{
    return delay.isInfiniteDelay() ? portMAX_DELAY : delay.getMs() / portTICK_RATE_MS;
}

constexpr uint32_t delayToMsCMSIS(DelayMs delay)
{
    return delay.isInfiniteDelay() ? osWaitForever : delay.getMs();
}

#endif // FREERTOSUTILS_HPP_
