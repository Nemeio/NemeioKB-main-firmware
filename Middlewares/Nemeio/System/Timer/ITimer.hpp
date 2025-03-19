/*
 * ITimer.hpp
 *
 *  Created on: 3 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_ITIMER_HPP_
#define NEMEIO_SYSTEM_TIMER_ITIMER_HPP_

#include "ITimerListener.hpp"
#include "SystemTypes.hpp"

class ITimer
{
public:
    enum class RetCode
    {
        SUCCESS,
        INVALID_PARAMETER,
        TIMER
    };

    ITimer() = default;
    virtual ~ITimer() = default;

    virtual void registerSingleListener(ITimerListener& listener) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual RetCode setDelay(const DelayMs& delay) = 0;
};

#endif /* NEMEIO_SYSTEM_TIMER_ITIMER_HPP_ */
