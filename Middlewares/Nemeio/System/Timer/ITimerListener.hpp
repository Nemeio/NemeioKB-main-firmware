/*
 * ITimerListener.hpp
 *
 *  Created on: 3 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_ITIMERLISTENER_HPP_
#define NEMEIO_SYSTEM_TIMER_ITIMERLISTENER_HPP_

class ITimerListener
{
public:
    ITimerListener() = default;
    virtual ~ITimerListener() = default;

    virtual void timerCallback() = 0;
};

#endif /* NEMEIO_SYSTEM_TIMER_ITIMERLISTENER_HPP_ */
