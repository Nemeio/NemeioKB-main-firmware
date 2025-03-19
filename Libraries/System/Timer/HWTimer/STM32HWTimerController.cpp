/*
 * STM32HWTimerController.cpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#include "STM32HWTimerController.hpp"
#include "STM32HWTimerUtil.hpp"

STM32HWTimerController::STM32HWTimerController(TIM_TypeDef* instance)
    : mHTIM({})
    , mInstance(instance)
{
}

bool STM32HWTimerController::init(uint32_t period, uint32_t freq)
{
    uint32_t apbFreq = STM32HWTimerUtil::getTimerSourceClockFreq(mInstance);

    mHTIM.Instance = mInstance;

    mHTIM.Init.Period = period - 1;
    mHTIM.Init.Prescaler = (apbFreq / freq) - 1;
    mHTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    mHTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
    mHTIM.Init.RepetitionCounter = 0;
    mHTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    return HAL_OK == HAL_TIM_Base_Init(&mHTIM);
}

bool STM32HWTimerController::start()
{
    // UIF flag needs to be cleared before starting the timer
    // to avoid TIM_IT_UPDATE interrupt firing immediately
    __HAL_TIM_CLEAR_FLAG(&mHTIM, TIM_SR_UIF);

    return HAL_OK == HAL_TIM_Base_Start_IT(&mHTIM);
}

bool STM32HWTimerController::stop()
{
    return HAL_OK == HAL_TIM_Base_Stop_IT(&mHTIM);
}

void STM32HWTimerController::irqHandler()
{
    HAL_TIM_IRQHandler(&mHTIM);
}

STM32HWTimerDefs::TimerId STM32HWTimerController::getTimerId()
{
    return STM32HWTimerUtil::getTimerIdFromInstance(mInstance);
}
