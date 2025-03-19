/*
 * STM32HWTimer.hpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#ifndef SYSTEM_TIMER_STM32HWTIMER_HPP_
#define SYSTEM_TIMER_STM32HWTIMER_HPP_

#include "AbstractHWTimer.hpp"
#include "ISTM32HWTimerController.hpp"
#include "AbstractRecursiveMutex.hpp"

class STM32HWTimer : public AbstractHWTimer
{
public:
    STM32HWTimer(ISTM32HWTimerController& timController, AbstractRecursiveMutex& mutex);
    STM32HWTimer(STM32HWTimer&) = delete;
    STM32HWTimer& operator=(const STM32HWTimer&) = delete;
    STM32HWTimer(STM32HWTimer&&) = delete;
    STM32HWTimer const& operator=(STM32HWTimer&&) = delete;
    virtual ~STM32HWTimer();

    void start();
    void stop();
    RetCode setDelay(const DelayMs& delay);
    RetCode setDelayUs(const DelayUs& delay);

    static void irqHandler(STM32HWTimerDefs::TimerId timerId);
    static void periodElapsedCallback(STM32HWTimerDefs::TimerId timerId);

private:
    static STM32HWTimer* sSTM32HWTimers[STM32HWTimerDefs::TimerId::MAX];

    bool mbInitialized;
    DelayUs mCurrentDelay;
    ISTM32HWTimerController& mTimController;

    AbstractRecursiveMutex& mMutex;

    void irqHandler();
    void periodElapsedCallback();

    static STM32HWTimer* getTimerFromTimerId(STM32HWTimerDefs::TimerId timerId);

    RetCode init(uint32_t delayUs);
};

#endif /* SYSTEM_TIMER_STM32HWTIMER_HPP_ */
