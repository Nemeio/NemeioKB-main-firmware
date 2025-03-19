/*
 * InvertedLED.hpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_LEDS_INVERTEDLED_HPP_
#define NEMEIO_LEDS_INVERTEDLED_HPP_

#include "AbstractLED.hpp"

class InvertedLED : public AbstractLED
{
public:
    InvertedLED(IGPIOController& gpioController, GPIOPin ledPin);
    virtual ~InvertedLED() = default;

    void turnOn();
    void turnOff();
};

#endif /* NEMEIO_LEDS_INVERTEDLED_HPP_ */
