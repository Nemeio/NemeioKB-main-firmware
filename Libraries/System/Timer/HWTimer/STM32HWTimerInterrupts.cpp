/*
 * STM32HWTimerInterrupts.cpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#include "STM32HWTimer.hpp"
#include "STM32HWTimerUtil.hpp"

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
extern "C" void TIM3_IRQHandler(void)
{
    STM32HWTimer::irqHandler(STM32HWTimerUtil::getTimerIdFromInstance(TIM3));
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    ASSERT(htim != nullptr);

    STM32HWTimer::periodElapsedCallback(STM32HWTimerUtil::getTimerIdFromInstance(htim->Instance));
}
