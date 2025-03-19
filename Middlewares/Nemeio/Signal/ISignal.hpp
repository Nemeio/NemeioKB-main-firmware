/*
 * ISignal.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SIGNAL_ISIGNAL_HPP_
#define NEMEIO_SIGNAL_ISIGNAL_HPP_

#include "SystemTypes.hpp"

class ISignal
{
public:
    virtual ~ISignal() = default;

    virtual bool wait(DelayMs delay) = 0;
    virtual void clear() = 0;
    virtual void setFromISR() = 0;
    virtual void set() = 0;
    virtual void set(DelayMs delay) = 0;
};

#endif /* NEMEIO_SIGNAL_ISIGNAL_HPP_ */
