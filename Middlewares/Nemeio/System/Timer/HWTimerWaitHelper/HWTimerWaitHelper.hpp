/*
 * HWTimerWaitHelper.hpp
 *
 *  Created on: 26 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_HWTIMERWAITHELPER_HWTIMERWAITHELPER_HPP_
#define NEMEIO_SYSTEM_TIMER_HWTIMERWAITHELPER_HWTIMERWAITHELPER_HPP_

#include "AbstractHWTimer.hpp"
#include "ITimerListener.hpp"
#include "ISignal.hpp"

class HWTimerWaitHelper : public ITimerListener
{
public:
    enum class RetCode
    {
        SUCCESS,
        INVALID_PARAMETER,
        TIMER,
        TIMEOUT
    };

    static constexpr int32_t DEFAULT_WAIT_MAX_TIMEOUT_OS_DELAY_MS = 10;

    HWTimerWaitHelper(AbstractHWTimer& timer, ISignal& signal);
    virtual ~HWTimerWaitHelper() = default;

    RetCode wait(const DelayUs& delay,
                 const DelayMs& maxTimeoutOsDelay = DelayMs(DEFAULT_WAIT_MAX_TIMEOUT_OS_DELAY_MS));

    void timerCallback();

private:
    AbstractHWTimer& mTimer;
    ISignal& mSignal;
};

#endif /* NEMEIO_SYSTEM_TIMER_HWTIMERWAITHELPER_HWTIMERWAITHELPER_HPP_ */
