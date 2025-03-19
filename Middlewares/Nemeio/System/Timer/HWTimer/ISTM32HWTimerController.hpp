/*
 * ISTM32HWTimerController.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_ISTM32HWTIMERCONTROLLER_HPP_
#define NEMEIO_SYSTEM_TIMER_ISTM32HWTIMERCONTROLLER_HPP_

#include "STM32HWTimerDefs.hpp"

class ISTM32HWTimerController
{
public:
    ISTM32HWTimerController() = default;
    virtual ~ISTM32HWTimerController() = default;

    virtual bool init(uint32_t period, uint32_t freq) = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;

    virtual void irqHandler() = 0;

    virtual STM32HWTimerDefs::TimerId getTimerId() = 0;
};

#endif /* NEMEIO_SYSTEM_TIMER_ISTM32HWTIMERCONTROLLER_HPP_ */
