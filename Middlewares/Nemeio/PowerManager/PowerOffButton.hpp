/*
 * PowerOffButton.hpp
 *
 *  Created on: 21 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_POWERMANAGER_POWEROFFBUTTON_HPP_
#define NEMEIO_POWERMANAGER_POWEROFFBUTTON_HPP_

#include "IPowerManager.hpp"
#include "IGPIOController.hpp"
#include "ISystem.hpp"

class PowerOffButton
{
public:
    static constexpr int32_t POWER_OFF_PRESS_DETECT_MIN_DELAY_MS = 150;

    PowerOffButton(IPowerManager& powerManager, IGPIOController& gpioController, ISystem& system);
    virtual ~PowerOffButton() = default;

    void init();

private:
    void vPowerInterruptCallback();

    IPowerManager& mPowerManager;
    IGPIOController& mGPIOController;
    ISystem& mSystem;

    bool mbPowerOffHasBeenPressed;
    uint32_t mPowerOffPressedTick;
};

#endif /* NEMEIO_POWERMANAGER_POWEROFFBUTTON_HPP_ */
