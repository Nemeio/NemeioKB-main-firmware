/*
 * STM32HWTimerController.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef SYSTEM_TIMER_STM32HWTIMERCONTROLLER_HPP_
#define SYSTEM_TIMER_STM32HWTIMERCONTROLLER_HPP_

#include "ISTM32HWTimerController.hpp"
#include "tim.h"

class STM32HWTimerController : public ISTM32HWTimerController
{
public:
    STM32HWTimerController(TIM_TypeDef* instance);
    virtual ~STM32HWTimerController() = default;

    bool init(uint32_t period, uint32_t freq);
    bool start();
    bool stop();

    void irqHandler();

    STM32HWTimerDefs::TimerId getTimerId();

private:
    TIM_HandleTypeDef mHTIM;
    TIM_TypeDef* mInstance;
};

#endif /* SYSTEM_TIMER_STM32HWTIMERCONTROLLER_HPP_ */
