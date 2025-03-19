/*
 * Timer.cpp
 *
 *  Created on: 3 avr. 2020
 *      Author: eskoric
 */

#include "Timer.hpp"
#include "FreeRTOSUtils.hpp"

Timer::Timer()
    : Timer(DelayMs(0))
{
}

Timer::Timer(const DelayMs delay)
    : mDelay(delay)
{
    osTimerStaticDef(timer, timerCallback, &mStaticTimer);

    mTimerId = osTimerCreate(osTimer(timer), osTimerOnce, this);
    ASSERT(mTimerId != nullptr);
}

Timer::~Timer()
{
    ASSERT(mTimerId != nullptr);
    osTimerDelete(mTimerId);
}

void Timer::start()
{
    ASSERT(mTimerId != nullptr);
    if(DelayMs(0) != mDelay)
    {
        osTimerStart(mTimerId, delayToMsCMSIS(mDelay));
    }
}

void Timer::stop()
{
    ASSERT(mTimerId != nullptr);
    osTimerStop(mTimerId);
}

Timer::RetCode Timer::setDelay(const DelayMs& delay)
{
    mDelay = delay;

    return RetCode::SUCCESS;
}

void Timer::timerCallback(const void* timerId)
{
    // Issue in CMSIS FreeRTOS v1 -> callback argument is the (non const) FreeRTOS timer handle
    // We need ugly casts to retrieve the Timer * pointer
    osTimerId id = reinterpret_cast<osTimerId>(const_cast<void*>(timerId));
    Timer* pTimer = reinterpret_cast<Timer*>(pvTimerGetTimerID(id));

    if(pTimer != nullptr && pTimer->mpTimerListener != nullptr)
    {
        pTimer->mpTimerListener->timerCallback();
    }
}
