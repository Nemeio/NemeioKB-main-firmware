/*
 * LED.hpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_LEDS_LED_HPP_
#define NEMEIO_LEDS_LED_HPP_

#include "AbstractLED.hpp"

class LED : public AbstractLED
{
public:
    LED(IGPIOController& gpioController, GPIOPin ledPin);
    virtual ~LED() = default;

    void turnOn();
    void turnOff();
};

#endif /* NEMEIO_LEDS_LED_HPP_ */
