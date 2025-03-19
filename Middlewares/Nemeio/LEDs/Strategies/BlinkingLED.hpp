/*
 * BlinkingLED.hpp
 *
 *  Created on: 3 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_LEDS_BLINKINGLED_HPP_
#define NEMEIO_LEDS_BLINKINGLED_HPP_

#include <atomic>
#include "IBlinkingLED.hpp"
#include "ITimerListener.hpp"
#include "ITimer.hpp"
#include "ILEDStrategy.hpp"
#include "cpp_helper.hpp"

class BlinkingLED : public ILEDStrategy, public ITimerListener
{
public:
    BlinkingLED(ILED& led, ITimer& timer, const DelayMs& delayOn, const DelayMs& delayOff);
    BlinkingLED(BlinkingLED&& led) noexcept;
    COPYABLE(BlinkingLED, delete);

    virtual ~BlinkingLED() = default;
    BlinkingLED& operator=(const BlinkingLED&& led) noexcept;

    void startLedStrategy() override;
    void stopLedStrategy() override;

    void timerCallback() override;

private:
    ILED& mLED;
    ITimer& mTimer;
    static_assert(std::atomic<DelayMs>::is_always_lock_free);
    std::atomic<DelayMs> mDelayOn;
    std::atomic<DelayMs> mDelayOff;
    bool mbOn = false;
};

#endif /* NEMEIO_LEDS_BLINKINGLED_HPP_ */
