/*
 * AlternateLED.cpp
 *
 *  Created on: 8 mars 2021
 *      Author: thomas
 */

#include "Strategies/AlternateLED.hpp"

AlternateLED::AlternateLED(ILED& firstLed, ILED& secondLed, ITimer& timer)
    : mFirstLED(firstLed)
    , mSecondLED(secondLed)
    , mTimer(timer)
{
    mTimer.registerSingleListener(*this);
}

AlternateLED::AlternateLED(AlternateLED&& led) noexcept
    : mFirstLED(led.mFirstLED)
    , mSecondLED(led.mSecondLED)
    , mTimer(led.mTimer)
    , mbFirstLedOn(led.mbFirstLedOn)
{
    mTimer.registerSingleListener(*this);
}

AlternateLED& AlternateLED::operator=(const AlternateLED&& led) noexcept
{
    mFirstLED = led.mFirstLED;
    mSecondLED = led.mSecondLED;
    mTimer = led.mTimer;
    mbFirstLedOn = led.mbFirstLedOn;

    mTimer.registerSingleListener(*this);

    return (*this);
}

void AlternateLED::turnOnFirstLed()
{
    mSecondLED.turnOff();
    mFirstLED.turnOn();
}

void AlternateLED::turnOnSecondLed()
{
    mFirstLED.turnOff();
    mSecondLED.turnOn();
}

void AlternateLED::timerCallback()
{
    if(mbFirstLedOn)
    {
        turnOnSecondLed();
    }
    else
    {
        turnOnFirstLed();
    }

    mbFirstLedOn = !mbFirstLedOn;
    mTimer.start();
}

void AlternateLED::startLedStrategy()
{
    turnOnFirstLed();
    mbFirstLedOn = true;
    mTimer.start();
}

void AlternateLED::stopLedStrategy()
{
    mTimer.stop();
    mFirstLED.turnOff();
    mSecondLED.turnOff();
}
