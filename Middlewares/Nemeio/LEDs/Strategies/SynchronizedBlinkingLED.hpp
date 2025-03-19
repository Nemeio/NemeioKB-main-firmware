/*
 * SynchronizedBlinkingLED.hpp
 *
 *  Created on: 18 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_LEDS_STRATEGIES_SYNCHRONIZEDBLINKINGLED_HPP_
#define NEMEIO_LEDS_STRATEGIES_SYNCHRONIZEDBLINKINGLED_HPP_

#include <atomic>
#include "ILED.hpp"
#include "SystemTypes.hpp"
#include "ITimerListener.hpp"
#include "ITimer.hpp"
#include "ILEDStrategy.hpp"
#include "cpp_helper.hpp"


class SynchronizedBlinkingLED : public ILEDStrategy, public ITimerListener
{
public:
    SynchronizedBlinkingLED(ILED& firstLed, ILED& secondLed, ITimer& timer);
    SynchronizedBlinkingLED(SynchronizedBlinkingLED&& led) noexcept;
    virtual ~SynchronizedBlinkingLED() = default;

    COPYABLE(SynchronizedBlinkingLED, delete);

    SynchronizedBlinkingLED& operator=(const SynchronizedBlinkingLED&& led) noexcept;

    void startLedStrategy() override;
    void stopLedStrategy() override;

    void timerCallback() override;

private:
    ILED& mFirstLED;
    ILED& mSecondLED;

    ITimer& mTimer;
    static_assert(std::atomic<DelayMs>::is_always_lock_free);
    bool mbLedsOn = false;

    void turnOnLeds();
    void turnOffLeds();
};

#endif /* NEMEIO_LEDS_STRATEGIES_SYNCHRONIZEDBLINKINGLED_HPP_ */
