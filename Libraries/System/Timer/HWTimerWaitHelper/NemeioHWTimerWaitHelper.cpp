/*
 * NemeioHWTimerWaitHelper.cpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#include "NemeioHWTimerWaitHelper.hpp"

NemeioHWTimerWaitHelper::NemeioHWTimerWaitHelper(AbstractHWTimer& timer)
    : mWaitHelper(timer, mSignal)
{
}

HWTimerWaitHelper::RetCode NemeioHWTimerWaitHelper::wait(const DelayUs& delay,
                                                         const DelayMs& maxTimeoutOsDelay)
{
    return mWaitHelper.wait(delay, maxTimeoutOsDelay);
}
