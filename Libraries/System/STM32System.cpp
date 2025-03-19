/*
 * STM32System.cpp
 *
 *  Created on: 12 févr. 2021
 *      Author: thomas
 */

#include "STM32System.hpp"
#include "systemUtils.h"
#include "cmsis_os.h"
#include "FreeRTOSUtils.hpp"

void STM32System::reset()
{
    SystemReset();
}

uint32_t STM32System::getTickCount()
{
    return osKernelSysTick();
}

uint32_t STM32System::delayToTicks(const DelayMs& delay)
{
    return delayToSystemTicks(delay);
}

void STM32System::delay(const DelayMs& delay)
{
    uint32_t millisec = delayToMsCMSIS(delay);

    if(millisec < osWaitForever)
    {
        ++millisec;
    }

    osDelay(millisec);
}

void STM32System::disableInterrupt()
{
    __disable_irq();
}

void STM32System::enableInterrupt()
{
    __enable_irq();
}
