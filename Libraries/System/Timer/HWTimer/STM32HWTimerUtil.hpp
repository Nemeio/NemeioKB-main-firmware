/*
 * STM32HWTimerUtil.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef SYSTEM_TIMER_STM32HWTIMERUTIL_HPP_
#define SYSTEM_TIMER_STM32HWTIMERUTIL_HPP_

#include "STM32HWTimerDefs.hpp"
#include "tim.h"

namespace STM32HWTimerUtil
{
uint32_t getTimerSourceClockFreq(TIM_TypeDef* instance);

STM32HWTimerDefs::TimerId getTimerIdFromInstance(TIM_TypeDef* instance);
} // namespace STM32HWTimerUtil


#endif /* SYSTEM_TIMER_STM32HWTIMERUTIL_HPP_ */
