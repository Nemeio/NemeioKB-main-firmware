/*
 * PowerOffButton.cpp
 *
 *  Created on: 21 avr. 2020
 *      Author: eskoric
 */

#include "PowerOffButton.hpp"

PowerOffButton::PowerOffButton(IPowerManager& powerManager,
                               IGPIOController& gpioController,
                               ISystem& system)
    : mPowerManager(powerManager)
    , mGPIOController(gpioController)
    , mSystem(system)
    , mbPowerOffHasBeenPressed(false)
    , mPowerOffPressedTick(0)
{
}

void PowerOffButton::init()
{
    std::function<void()> powerItCallback = std::bind(&PowerOffButton::vPowerInterruptCallback,
                                                      this);

    GPIOInConfig gpioInConfig;
    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_NO_PULL;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING_FALLING;
    gpioInConfig.itCallback = powerItCallback;

#ifndef USE_ETM
    mGPIOController.configureInput(WKup_3, gpioInConfig);
#endif
}

void PowerOffButton::vPowerInterruptCallback()
{
    bool bButtonPressed = (mGPIOController.read(WKup_3) == IGPIO_STATE_LOW);
    uint32_t itTick = mSystem.getTickCount();

    if(bButtonPressed)
    {
        mPowerOffPressedTick = itTick;
    }
    else if(mbPowerOffHasBeenPressed && itTick > mPowerOffPressedTick
            && (itTick - mPowerOffPressedTick)
                   >= mSystem.delayToTicks(DelayMs(POWER_OFF_PRESS_DETECT_MIN_DELAY_MS)))
    {
        mPowerManager.powerOff();
    }

    mbPowerOffHasBeenPressed = bButtonPressed;
}
