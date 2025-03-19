/*
 * AbstractLED.hpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_LEDS_ABSTRACTLED_HPP_
#define NEMEIO_LEDS_ABSTRACTLED_HPP_

#include "ILED.hpp"
#include "IGPIOController.hpp"

class AbstractLED : public ILED
{
public:
    AbstractLED(IGPIOController& gpioController, GPIOPin ledPin);
    virtual ~AbstractLED() = default;

    void init(const GPIOOutConfig& config);

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;

protected:
    IGPIOController& mGPIOController;
    GPIOPin mLEDPin;
};

#endif /* NEMEIO_LEDS_ABSTRACTLED_HPP_ */
