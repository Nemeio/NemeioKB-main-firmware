/*
 * AbstractLED.cpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#include "AbstractLED.hpp"

AbstractLED::AbstractLED(IGPIOController& gpioController, GPIOPin ledPin)
    : mGPIOController(gpioController)
    , mLEDPin(ledPin)
{
}

void AbstractLED::init(const GPIOOutConfig& config)
{
    mGPIOController.configureOutput(mLEDPin, config);

    turnOff();
}
