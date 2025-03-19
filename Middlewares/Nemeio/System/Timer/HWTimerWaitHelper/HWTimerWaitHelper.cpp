/*
 * HWTimerWaitHelper.cpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#include "HWTimerWaitHelper.hpp"

HWTimerWaitHelper::HWTimerWaitHelper(AbstractHWTimer& timer, ISignal& signal)
    : mTimer(timer)
    , mSignal(signal)
{
    mTimer.registerSingleListener(*this);
}

HWTimerWaitHelper::RetCode HWTimerWaitHelper::wait(const DelayUs& delay,
                                                   const DelayMs& maxTimeoutOsDelay)
{
    if(maxTimeoutOsDelay <= delay)
    {
        return RetCode::INVALID_PARAMETER;
    }

    ITimer::RetCode timerRet = mTimer.setDelayUs(delay);
    RetCode ret = RetCode::SUCCESS;

    if(ITimer::RetCode::INVALID_PARAMETER == timerRet)
    {
        ret = RetCode::INVALID_PARAMETER;
    }
    else if(ITimer::RetCode::TIMER == timerRet)
    {
        ret = RetCode::TIMER;
    }

    if(RetCode::SUCCESS == ret)
    {
        mTimer.stop();
        mSignal.clear();
        mTimer.start();

        if(mSignal.wait(maxTimeoutOsDelay))
        {
            ret = RetCode::SUCCESS;
        }
        else
        {
            mTimer.stop();
            ret = RetCode::TIMEOUT;
        }
    }

    return ret;
}

void HWTimerWaitHelper::timerCallback()
{
    mSignal.setFromISR();
}
