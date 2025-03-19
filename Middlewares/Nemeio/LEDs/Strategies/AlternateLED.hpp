/*
 * AlternateLED.hpp
 *
 *  Created on: 8 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_LEDS_STRATEGIES_ALTERNATELED_HPP_
#define NEMEIO_LEDS_STRATEGIES_ALTERNATELED_HPP_

#include <atomic>
#include "ILED.hpp"
#include "SystemTypes.hpp"
#include "ITimerListener.hpp"
#include "ITimer.hpp"
#include "ILEDStrategy.hpp"
#include "cpp_helper.hpp"

class AlternateLED : public ILEDStrategy, public ITimerListener
{
public:
    AlternateLED(ILED& firstLed, ILED& secondLed, ITimer& timer);
    AlternateLED(AlternateLED&& led) noexcept;

    COPYABLE(AlternateLED, delete);

    virtual ~AlternateLED() = default;

    AlternateLED& operator=(const AlternateLED&& led) noexcept;

    void startLedStrategy() override;
    void stopLedStrategy() override;

    void timerCallback() override;

private:
    ILED& mFirstLED;
    ILED& mSecondLED;

    ITimer& mTimer;
    static_assert(std::atomic<DelayMs>::is_always_lock_free);
    bool mbFirstLedOn = true;

    void turnOnFirstLed();
    void turnOnSecondLed();
};

#endif /* NEMEIO_LEDS_STRATEGIES_ALTERNATELED_HPP_ */
