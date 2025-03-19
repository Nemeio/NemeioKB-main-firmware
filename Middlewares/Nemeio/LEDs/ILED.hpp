/*
 * ILED.hpp
 *
 *  Created on: 26 mars 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_LEDS_ILED_HPP_
#define NEMEIO_LEDS_ILED_HPP_

class ILED
{
public:
    ILED() = default;
    virtual ~ILED() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};

#endif /* NEMEIO_LEDS_ILED_HPP_ */
