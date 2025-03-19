/*
 * STM32HWTimer.cpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#include "STM32HWTimer.hpp"
#include "AutoLock.h"

STM32HWTimer* STM32HWTimer::sSTM32HWTimers[STM32HWTimerDefs::TimerId::MAX] = {0};

STM32HWTimer::STM32HWTimer(ISTM32HWTimerController& timController, AbstractRecursiveMutex& mutex)
    : mbInitialized(false)
    , mCurrentDelay(0)
    , mTimController(timController)
    , mMutex(mutex)
{
    STM32HWTimerDefs::TimerId timerIdx = mTimController.getTimerId();

    if(timerIdx < STM32HWTimerDefs::TimerId::MAX)
    {
        sSTM32HWTimers[static_cast<std::size_t>(timerIdx)] = this;
    }
}

STM32HWTimer::~STM32HWTimer()
{
    STM32HWTimerDefs::TimerId timerIdx = mTimController.getTimerId();

    if(timerIdx < STM32HWTimerDefs::TimerId::MAX)
    {
        sSTM32HWTimers[static_cast<std::size_t>(timerIdx)] = nullptr;
    }
}

void STM32HWTimer::irqHandler(STM32HWTimerDefs::TimerId timerId)
{
    STM32HWTimer* pTimer = STM32HWTimer::getTimerFromTimerId(timerId);

    if(pTimer != nullptr)
    {
        pTimer->irqHandler();
    }
}

void STM32HWTimer::periodElapsedCallback(STM32HWTimerDefs::TimerId timerId)
{
    STM32HWTimer* pTimer = STM32HWTimer::getTimerFromTimerId(timerId);

    if(pTimer != nullptr)
    {
        pTimer->periodElapsedCallback();
    }
}

ITimer::RetCode STM32HWTimer::init(uint32_t delayUs)
{
    AutoLock autoLock(mMutex);

    RetCode ret = mTimController.init(delayUs, STM32HWTimerDefs::TIMER_FREQ_HZ) ? RetCode::SUCCESS
                                                                                : RetCode::TIMER;

    if(RetCode::SUCCESS == ret)
    {
        mbInitialized = true;
    }

    return ret;
}

void STM32HWTimer::start()
{
    AutoLock autoLock(mMutex);

    if(!mbInitialized)
    {
        return;
    }

    mTimController.start();
}

void STM32HWTimer::stop()
{
    AutoLock autoLock(mMutex);

    if(!mbInitialized)
    {
        return;
    }

    mTimController.stop();
}

ITimer::RetCode STM32HWTimer::setDelay(const DelayMs& delay)
{
    return setDelayUs(delay);
}

ITimer::RetCode STM32HWTimer::setDelayUs(const DelayUs& delay)
{
    int32_t valueUs = delay.valueUs;

    if(valueUs < STM32HWTimerDefs::TIMER_MIN_DELAY || valueUs > STM32HWTimerDefs::TIMER_MAX_DELAY)
    {
        return RetCode::INVALID_PARAMETER;
    }

    RetCode ret = RetCode::SUCCESS;

    if(!mbInitialized || mCurrentDelay != delay)
    {
        ret = init(valueUs);
        if(RetCode::SUCCESS == ret)
        {
            mCurrentDelay = delay;
        }
    }

    return ret;
}

void STM32HWTimer::irqHandler()
{
    mTimController.irqHandler();
}

STM32HWTimer* STM32HWTimer::getTimerFromTimerId(STM32HWTimerDefs::TimerId timerId)
{
    STM32HWTimer* pTimer = nullptr;

    if(timerId < STM32HWTimerDefs::TimerId::MAX)
    {
        pTimer = STM32HWTimer::sSTM32HWTimers[static_cast<std::size_t>(timerId)];
    }

    return pTimer;
}

void STM32HWTimer::periodElapsedCallback()
{
    mTimController.stop();

    if(mpTimerListener != nullptr)
    {
        mpTimerListener->timerCallback();
    }
}
