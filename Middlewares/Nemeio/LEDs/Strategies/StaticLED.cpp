/*
 * StaticLED.cpp
 *
 *  Created on: 6 mars 2021
 *      Author: thomas
 */

#include <Strategies/StaticLED.hpp>

StaticLED::StaticLED(ILED& led, staticLEDState state)
    : mLed(led)
    , mLedState(state)
{
}

StaticLED::StaticLED(StaticLED&& led) noexcept
    : mLed(led.mLed)
    , mLedState(led.mLedState)
{
}


StaticLED& StaticLED::operator=(const StaticLED&& led) noexcept
{
    mLedState = led.mLedState;
    mLed = led.mLed;

    return (*this);
}

void StaticLED::startLedStrategy()
{
    if(mLedState == staticLEDState::LED_ON)
    {
        mLed.turnOn();
    }
    else
    {
        mLed.turnOff();
    }
}
void StaticLED::stopLedStrategy()
{
    if(mLedState == staticLEDState::LED_ON)
    {
        mLed.turnOff();
    }
}
