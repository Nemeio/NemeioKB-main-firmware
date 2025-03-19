/*
 * BrightnessPowerStateHandler.cpp
 *
 *  Created on: 21 janv. 2022
 *      Author: thomas
 */

#include <BrightnessPowerStateHandler.hpp>

BrightnessPowerStateHandler::BrightnessPowerStateHandler(BrightnessManager& brightnessManager)
    : mBrightnessManager(brightnessManager)
{
}

void BrightnessPowerStateHandler::setBrightnessForPowerState(uint8_t brightnessPercentageActive,
                                                             uint8_t brightnessPercentageInactive)
{
    mActiveBrightnessPercent = brightnessPercentageActive;
    mInactiveBrightnessPercent = brightnessPercentageInactive;
}


void BrightnessPowerStateHandler::active()
{
    mBrightnessManager.setBrightnessPercentageProgressive(mActiveBrightnessPercent,
                                                          PROGRESSIVE_CHANGE_STEP_PERCENT,
                                                          PROGRESSIVE_CHANGE_DELAY_MS);
}

void BrightnessPowerStateHandler::inactive()
{
    mBrightnessManager.setBrightnessPercentageProgressive(mInactiveBrightnessPercent,
                                                          PROGRESSIVE_CHANGE_STEP_PERCENT,
                                                          PROGRESSIVE_CHANGE_DELAY_MS);
}

void BrightnessPowerStateHandler::longInactive()
{
    mBrightnessManager.setBrightnessPercentageProgressive(0,
                                                          PROGRESSIVE_CHANGE_STEP_PERCENT,
                                                          PROGRESSIVE_CHANGE_DELAY_MS);
}

void BrightnessPowerStateHandler::sleep()
{
}

void BrightnessPowerStateHandler::powerOff()
{
    mBrightnessManager.setBrightnessPercentageProgressive(0,
                                                          PROGRESSIVE_CHANGE_STEP_PERCENT,
                                                          PROGRESSIVE_CHANGE_DELAY_MS);
}

void BrightnessPowerStateHandler::suspend()
{
    mBrightnessManager.setBrightnessPercentageProgressive(0,
                                                          PROGRESSIVE_CHANGE_STEP_PERCENT,
                                                          PROGRESSIVE_CHANGE_DELAY_MS);
}
