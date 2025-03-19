/*
 * LED.cpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#include "LED.hpp"

LED::LED(IGPIOController& gpioController, GPIOPin ledPin)
    : AbstractLED(gpioController, ledPin)
{
}

void LED::turnOn()
{
    mGPIOController.set(mLEDPin);
}

void LED::turnOff()
{
    mGPIOController.clear(mLEDPin);
}
