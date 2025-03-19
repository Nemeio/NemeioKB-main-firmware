/*
 * BrightnessPowerStateHandler.hpp
 *
 *  Created on: 21 janv. 2022
 *      Author: thomas
 */

#ifndef FRONTLIGHT_BRIGHTNESSPOWERSTATEHANDLER_HPP_
#define FRONTLIGHT_BRIGHTNESSPOWERSTATEHANDLER_HPP_

#include "IPowerManageable.h"
#include "BrightnessManager.hpp"

class BrightnessPowerStateHandler : public IPowerManageable
{
public:
    explicit BrightnessPowerStateHandler(BrightnessManager& brightnessManager);
    virtual ~BrightnessPowerStateHandler() = default;

    void active() final;
    void inactive() final;
    void longInactive() final;
    void sleep() final;
    void powerOff() final;
    void suspend() final;

    void setBrightnessForPowerState(uint8_t brightnessPercentageActive,
                                    uint8_t brightnessPercentageInactive);

private:
    static constexpr uint8_t PROGRESSIVE_CHANGE_STEP_PERCENT = 2;
    static constexpr DelayMs PROGRESSIVE_CHANGE_DELAY_MS = 10;

    BrightnessManager& mBrightnessManager;
    uint8_t mActiveBrightnessPercent = 100;
    uint8_t mInactiveBrightnessPercent = 0;
};

#endif /* FRONTLIGHT_BRIGHTNESSPOWERSTATEHANDLER_HPP_ */
