/*
 * IBlinkingLED.hpp
 *
 *  Created on: 6 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_LEDS_IBLINKINGLED_HPP_
#define NEMEIO_LEDS_IBLINKINGLED_HPP_

#include "ILED.hpp"
#include "SystemTypes.hpp"

class IBlinkingLED : public ILED
{
public:
    IBlinkingLED() = default;
    virtual ~IBlinkingLED() = default;

    virtual void blinkStart() = 0;
    virtual void blinkStop() = 0;
    virtual void setBlinkTimings(const DelayMs& delayOn, const DelayMs& delayOff) = 0;
};

#endif /* NEMEIO_LEDS_IBLINKINGLED_HPP_ */
