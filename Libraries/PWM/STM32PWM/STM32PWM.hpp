/*
 * STM32PWM.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Labo
 */

#ifndef PWM_STM32PWM_STM32PWM_HPP_
#define PWM_STM32PWM_STM32PWM_HPP_

#include "IPWM.hpp"
#include "tim.h"
#include "cmsis_os.h"
#include "Mutex.hpp"

class STM32PWM : public IPWM
{
public:
    STM32PWM(TIM_TypeDef* instance, uint32_t channel);
    virtual ~STM32PWM();

    void init(uint32_t freqHz);
    void setDutyCycle(uint8_t dutyCycle);

private:
    TIM_HandleTypeDef mHTIM;
    TIM_TypeDef* mInstance;
    uint32_t mChannel;
    uint32_t mPeriod;

    Mutex mPWMMutex;
};

#endif /* PWM_STM32PWM_STM32PWM_HPP_ */
