/*
 * IPWM.h
 *
 *  Created on: Aug 2, 2018
 *      Author: Labo
 */

#ifndef PWM_IPWM_IPWM_HPP_
#define PWM_IPWM_IPWM_HPP_

#include <stdint.h>

class IPWM
{
public:
    virtual void init(uint32_t freqHz) = 0;
    virtual void setDutyCycle(uint8_t dutyCycle) = 0;
};

#endif /* PWM_IPWM_IPWM_HPP_ */
