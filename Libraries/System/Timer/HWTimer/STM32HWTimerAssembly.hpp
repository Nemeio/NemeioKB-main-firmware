/*
 * STM32HWTimerAssembly.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef SYSTEM_TIMER_STM32HWTIMERASSEMBLY_HPP_
#define SYSTEM_TIMER_STM32HWTIMERASSEMBLY_HPP_

#include "STM32HWTimerController.hpp"
#include "STM32HWTimer.hpp"
#include "RecursiveMutex.hpp"

class STM32HWTimerAssembly
{
public:
    STM32HWTimerAssembly(TIM_TypeDef* instance);
    virtual ~STM32HWTimerAssembly() = default;

    STM32HWTimer& getTimerItem();

private:
    RecursiveMutex mMutex;
    STM32HWTimerController mTimController;
    STM32HWTimer mTimer;
};

#endif /* SYSTEM_TIMER_STM32HWTIMERASSEMBLY_HPP_ */
