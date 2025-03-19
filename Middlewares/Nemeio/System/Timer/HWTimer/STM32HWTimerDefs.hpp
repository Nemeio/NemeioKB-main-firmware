/*
 * STM32HWTimerTools.hpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#ifndef SYSTEM_TIMER_STM32HWTIMERDEFS_HPP_
#define SYSTEM_TIMER_STM32HWTIMERDEFS_HPP_

#include <cstdint>

namespace STM32HWTimerDefs
{
enum TimerId
{
    TIM_3 = 0,
    MAX
};

inline constexpr uint32_t TIMER_FREQ_HZ = 1000000; // 1 MHz -> microsecond precision

inline constexpr int32_t TIMER_MIN_DELAY = 1;
inline constexpr int32_t TIMER_MAX_DELAY = 0x10000;
}; // namespace STM32HWTimerDefs

#endif /* SYSTEM_TIMER_STM32HWTIMERDEFS_HPP_ */
