/*
 * Timer.hpp
 *
 *  Created on: 3 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_TIMER_HPP_
#define NEMEIO_SYSTEM_TIMER_TIMER_HPP_

#include "AbstractTimer.hpp"
#include "cmsis_os.h"

class Timer : public AbstractTimer
{
public:
    Timer();
    explicit Timer(const DelayMs delay);
    virtual ~Timer();
    Timer(Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer const& operator=(Timer&&) = delete;

    void start();
    void stop();
    RetCode setDelay(const DelayMs& delay);

private:
    static void timerCallback(const void* timerId);

    osTimerId mTimerId;
    osTimerDef_t mTimerDef;
    osStaticTimerDef_t mStaticTimer;
    DelayMs mDelay;
};

#endif /* NEMEIO_SYSTEM_TIMER_TIMER_HPP_ */
