/*
 * STM32HWTimerUtil.cpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#include "STM32HWTimerUtil.hpp"
#include "assertTools.h"

uint32_t STM32HWTimerUtil::getTimerSourceClockFreq(TIM_TypeDef* instance)
{
    ASSERT(nullptr != instance);

    uint32_t apbFreq = 0;

    if(TIM2 == instance || TIM3 == instance || TIM4 == instance || TIM5 == instance
       || TIM6 == instance || TIM7 == instance)
    {
        apbFreq = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        apbFreq = HAL_RCC_GetPCLK2Freq();
    }

    return apbFreq;
}

STM32HWTimerDefs::TimerId STM32HWTimerUtil::getTimerIdFromInstance(TIM_TypeDef* instance)
{
    STM32HWTimerDefs::TimerId timerId = STM32HWTimerDefs::TimerId::MAX;

    if(TIM3 == instance)
    {
        timerId = STM32HWTimerDefs::TimerId::TIM_3;
    }

    return timerId;
}
