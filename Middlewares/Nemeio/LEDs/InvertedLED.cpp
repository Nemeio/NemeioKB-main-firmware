/*
 * InvertedLED.cpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#include "InvertedLED.hpp"

InvertedLED::InvertedLED(IGPIOController& gpioController, GPIOPin ledPin)
    : AbstractLED(gpioController, ledPin)
{
}

void InvertedLED::turnOn()
{
    mGPIOController.clear(mLEDPin);
}

void InvertedLED::turnOff()
{
    mGPIOController.set(mLEDPin);
}
