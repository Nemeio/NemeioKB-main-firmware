/*
 * BlinkingLED.cpp
 *
 *  Created on: 3 avr. 2020
 *      Author: eskoric
 */

#include "BlinkingLED.hpp"

BlinkingLED::BlinkingLED(ILED& led, ITimer& timer, const DelayMs& delayOn, const DelayMs& delayOff)
    : mLED(led)
    , mTimer(timer)
    , mDelayOn(delayOn)
    , mDelayOff(delayOff)
{
    mTimer.registerSingleListener(*this);
}

BlinkingLED::BlinkingLED(BlinkingLED&& led) noexcept
    : mLED(led.mLED)
    , mTimer(led.mTimer)
    , mDelayOn(led.mDelayOn.load())
    , mDelayOff(led.mDelayOff.load())
{
    mTimer.registerSingleListener(*this);
}

BlinkingLED& BlinkingLED::operator=(const BlinkingLED&& led) noexcept
{
    mLED = led.mLED;
    mTimer = led.mTimer;
    mDelayOn = led.mDelayOn.load();
    mDelayOff = led.mDelayOff.load();
    mbOn = led.mbOn;

    mTimer.registerSingleListener(*this);

    return (*this);
}

void BlinkingLED::startLedStrategy()
{
    mLED.turnOn();
    mbOn = true;
    mTimer.setDelay(mDelayOn);
    mTimer.start();
}

void BlinkingLED::stopLedStrategy()
{
    if(mbOn)
    {
        mLED.turnOff();
        mbOn = false;
    }
    mTimer.stop();
}

void BlinkingLED::timerCallback()
{
    if(mbOn)
    {
        mLED.turnOff();
        mTimer.setDelay(mDelayOff);
    }
    else
    {
        mLED.turnOn();
        mTimer.setDelay(mDelayOn);
    }
    mbOn = !mbOn;
    mTimer.start();
}
