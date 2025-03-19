/*
 * SingleChipSelector.cpp
 *
 *  Created on: May 12, 2017
 *      Author: Labo
 */

#include "SingleChipSelector.h"

SingleChipSelector::SingleChipSelector(IGPIOController& gpioController, GPIOPin csNPin)
    : mGPIOController(gpioController)
    , mCSNPin(csNPin)
{
    GPIOOutConfig gpioOutConfig = {IGPIO_OUTTYPE_PUSHPULL, IGPIO_NO_PULL};

    mGPIOController.configureOutput(mCSNPin, gpioOutConfig);
    mGPIOController.set(mCSNPin);
}

SingleChipSelector::~SingleChipSelector()
{
}

void SingleChipSelector::activate()
{
    mGPIOController.clear(mCSNPin);
}

void SingleChipSelector::deactivate()
{
    mGPIOController.set(mCSNPin);
}
