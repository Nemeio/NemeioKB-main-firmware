/*
 * BrightnessButtonsHandler.hpp
 *
 *  Created on: 14 janv. 2022
 *      Author: thomas
 */

#ifndef FRONTLIGHT_BRIGHTNESSBUTTONSHANDLER_HPP_
#define FRONTLIGHT_BRIGHTNESSBUTTONSHANDLER_HPP_

#include "IDoubleClickKeyListener.hpp"
#include "BrightnessManager.hpp"
#include "IBrightnessListener.hpp"

#include "etl/vector.h"

class BrightnessButtonsHandler : public IDoubleClickKeyListener
{
public:
    static constexpr size_t BRIGHTNESS_MAX_STEP = 256;

    explicit BrightnessButtonsHandler(BrightnessManager& brightnessManager,
                                      IBrightnessListener& brightnessListener);
    virtual ~BrightnessButtonsHandler() = default;

    void setBrightnessStepList(const etl::vector<uint8_t, BRIGHTNESS_MAX_STEP>& steps,
                               uint8_t currentPos);

    void simpleClicked(const ButtonID& button) final;
    void doubleClicked(const ButtonID& button) final;

private:
    static constexpr ButtonID BRIGHTNESS_CHANGE_BTN{ButtonID(BTN_UP)};

    enum class ChangeDirection
    {
        INCREASE,
        DECREASE
    };

    BrightnessManager& mBrightnessManager;
    IBrightnessListener& mBrightnessListener;

    using brightnessSteps = etl::vector<uint8_t, BRIGHTNESS_MAX_STEP>;
    brightnessSteps mBrightnessStepList;
    brightnessSteps::iterator mBrightnessIterator;

    void changeBrightnessButtonPressed(ChangeDirection direction);
    uint8_t getNextBrightnessPercentage();
    uint8_t getPreviousBrightnessPercentage();
};

#endif /* FRONTLIGHT_BRIGHTNESSBUTTONSHANDLER_HPP_ */
