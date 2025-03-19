/*
 * PAM2804Controller.hpp
 *
 *  Created on: Aug 2, 2018
 *      Author: Labo
 */

#ifndef FRONTLIGHT_PAM2804_PAM2804CONTROLLER_HPP_
#define FRONTLIGHT_PAM2804_PAM2804CONTROLLER_HPP_

#include "IPWM.hpp"
#include "ILedController.hpp"


class PAM2804Controller : public ILedController
{
public:
    explicit PAM2804Controller(IPWM& pwm);
    virtual ~PAM2804Controller() = default;

    bool setLightIntensity(uint8_t intensityPercent) override;
    void init() override;
    void powerOff() override;

private:
    IPWM& mPWM;
};

#endif /* FRONTLIGHT_PAM2804_PAM2804CONTROLLER_HPP_ */
