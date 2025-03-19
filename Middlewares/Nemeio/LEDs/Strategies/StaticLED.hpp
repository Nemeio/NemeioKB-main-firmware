/*
 * StaticLED.hpp
 *
 *  Created on: 6 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_LEDS_STRATEGIES_STATICLED_HPP_
#define NEMEIO_LEDS_STRATEGIES_STATICLED_HPP_

#include "ILED.hpp"
#include "ILEDStrategy.hpp"
#include "cpp_helper.hpp"

enum class staticLEDState
{
    LED_ON,
    LED_OFF,
};

class StaticLED : public ILEDStrategy
{
public:
    StaticLED(ILED& led, staticLEDState state);
    StaticLED(StaticLED&& led) noexcept;
    COPYABLE(StaticLED, delete);

    virtual ~StaticLED() = default;

    StaticLED& operator=(const StaticLED&& led) noexcept;

    void startLedStrategy() override;
    void stopLedStrategy() override;

private:
    ILED& mLed;
    staticLEDState mLedState;
};

#endif /* NEMEIO_LEDS_STRATEGIES_STATICLED_HPP_ */
