/*
 * NemeioHWTimerWaitHelper.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef SYSTEM_TIMER_HWTIMERWAITHELPER_NEMEIOHWTIMERWAITHELPER_HPP_
#define SYSTEM_TIMER_HWTIMERWAITHELPER_NEMEIOHWTIMERWAITHELPER_HPP_

#include "HWTimerWaitHelper.hpp"
#include "Signal.hpp"

class NemeioHWTimerWaitHelper
{
public:
    NemeioHWTimerWaitHelper(AbstractHWTimer& timer);
    virtual ~NemeioHWTimerWaitHelper() = default;

    HWTimerWaitHelper::RetCode wait(const DelayUs& delay,
                                    const DelayMs& maxTimeoutOsDelay = DelayMs(
                                        HWTimerWaitHelper::DEFAULT_WAIT_MAX_TIMEOUT_OS_DELAY_MS));

private:
    Signal mSignal;
    HWTimerWaitHelper mWaitHelper;
};

#endif /* SYSTEM_TIMER_HWTIMERWAITHELPER_NEMEIOHWTIMERWAITHELPER_HPP_ */
