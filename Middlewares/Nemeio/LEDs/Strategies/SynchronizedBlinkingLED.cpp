/*
 * SynchronizedBlinkingLED.cpp
 *
 *  Created on: 18 janv. 2022
 *      Author: thomas
 */

#include "Strategies/SynchronizedBlinkingLED.hpp"

SynchronizedBlinkingLED::SynchronizedBlinkingLED(ILED& firstLed, ILED& secondLed, ITimer& timer)
    : mFirstLED(firstLed)
    , mSecondLED(secondLed)
    , mTimer(timer)
{
    mTimer.registerSingleListener(*this);
}

SynchronizedBlinkingLED::SynchronizedBlinkingLED(SynchronizedBlinkingLED&& led) noexcept
    : mFirstLED(led.mFirstLED)
    , mSecondLED(led.mSecondLED)
    , mTimer(led.mTimer)
    , mbLedsOn(led.mbLedsOn)
{
    mTimer.registerSingleListener(*this);
}

SynchronizedBlinkingLED& SynchronizedBlinkingLED::operator=(
    const SynchronizedBlinkingLED&& led) noexcept
{
    mFirstLED = led.mFirstLED;
    mSecondLED = led.mSecondLED;
    mTimer = led.mTimer;
    mbLedsOn = led.mbLedsOn;

    mTimer.registerSingleListener(*this);

    return (*this);
}

void SynchronizedBlinkingLED::turnOnLeds()
{
    mSecondLED.turnOn();
    mFirstLED.turnOn();
}

void SynchronizedBlinkingLED::turnOffLeds()
{
    mFirstLED.turnOff();
    mSecondLED.turnOff();
}

void SynchronizedBlinkingLED::timerCallback()
{
    if(mbLedsOn)
    {
        turnOffLeds();
    }
    else
    {
        turnOnLeds();
    }

    mbLedsOn = !mbLedsOn;
    mTimer.start();
}

void SynchronizedBlinkingLED::startLedStrategy()
{
    turnOnLeds();
    mbLedsOn = true;
    mTimer.start();
}

void SynchronizedBlinkingLED::stopLedStrategy()
{
    mTimer.stop();
    mFirstLED.turnOff();
    mSecondLED.turnOff();
}
