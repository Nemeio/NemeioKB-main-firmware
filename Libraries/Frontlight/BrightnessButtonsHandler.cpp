/*
 * BrightnessButtonsHandler.cpp
 *
 *  Created on: 14 janv. 2022
 *      Author: thomas
 */

#include "BrightnessButtonsHandler.hpp"

BrightnessButtonsHandler::BrightnessButtonsHandler(BrightnessManager& brightnessManager,
                                                   IBrightnessListener& brightnessListener)
    : mBrightnessManager(brightnessManager)
    , mBrightnessListener(brightnessListener)
{
}

void BrightnessButtonsHandler::changeBrightnessButtonPressed(ChangeDirection direction)
{
    if(mBrightnessStepList.begin() == mBrightnessStepList.end())
    {
        return;
    }

    if(direction == ChangeDirection::INCREASE)
    {
        mBrightnessManager.setBrightnessPercentage(getNextBrightnessPercentage());
    }
    else if(direction == ChangeDirection::DECREASE)
    {
        mBrightnessManager.setBrightnessPercentage(getPreviousBrightnessPercentage());
    }

    mBrightnessListener.onBrightnessChanged(mBrightnessManager.getBrightnessPercentage());
}

void BrightnessButtonsHandler::simpleClicked(const ButtonID& button)
{
    if(button == BRIGHTNESS_CHANGE_BTN)
    {
        changeBrightnessButtonPressed(ChangeDirection::INCREASE);
    }
}

void BrightnessButtonsHandler::doubleClicked(const ButtonID& button)
{
    if(button == BRIGHTNESS_CHANGE_BTN)
    {
        changeBrightnessButtonPressed(ChangeDirection::DECREASE);
    }
}

uint8_t BrightnessButtonsHandler::getNextBrightnessPercentage()
{
    if(mBrightnessIterator >= (mBrightnessStepList.end() - 1))
    {
        return *mBrightnessIterator;
    }

    return *(++mBrightnessIterator);
}

uint8_t BrightnessButtonsHandler::getPreviousBrightnessPercentage()
{
    if(mBrightnessIterator == mBrightnessStepList.begin())
    {
        return (*mBrightnessIterator);
    }

    return *(--mBrightnessIterator);
}

void BrightnessButtonsHandler::setBrightnessStepList(
    const etl::vector<uint8_t, BRIGHTNESS_MAX_STEP>& steps, uint8_t currentPos)
{
    ASSERT(steps.size() > 0);
    ASSERT(currentPos < steps.size());

    mBrightnessStepList = steps;
    mBrightnessIterator = mBrightnessStepList.begin() + currentPos;
}
