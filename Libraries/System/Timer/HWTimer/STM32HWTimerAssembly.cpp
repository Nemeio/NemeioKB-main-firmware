/*
 * STM32HWTimerAssembly.cpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#include "STM32HWTimerAssembly.hpp"

STM32HWTimerAssembly::STM32HWTimerAssembly(TIM_TypeDef* instance)
    : mTimController(instance)
    , mTimer(mTimController, mMutex)
{
}

STM32HWTimer& STM32HWTimerAssembly::getTimerItem()
{
    return mTimer;
}
