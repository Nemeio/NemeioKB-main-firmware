/*
 * PAM2804Controller.cpp
 *
 *  Created on: Aug 2, 2018
 *      Author: Labo
 */

#include <PAM2804/PAM2804Controller.hpp>

static const uint16_t PAM2804_PWM_FREQ_HZ = 100;

PAM2804Controller::PAM2804Controller(IPWM& pwm)
    : mPWM(pwm)
{
}

bool PAM2804Controller::setLightIntensity(uint8_t intensityPercent)
{
    if(intensityPercent > 100)
    {
        intensityPercent = 100;
    }

    mPWM.setDutyCycle(intensityPercent);

    return true;
}

void PAM2804Controller::init()
{
    mPWM.init(PAM2804_PWM_FREQ_HZ);
}

void PAM2804Controller::powerOff()
{
    mPWM.setDutyCycle(0);
}
