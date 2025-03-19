/*
 * BrightnessDemoModeHandler.cpp
 *
 *  Created on: 24 janv. 2022
 *      Author: thomas
 */

#include <BrightnessDemoModeHandler.hpp>

BrightnessDemoModeHandler::BrightnessDemoModeHandler(BrightnessManager& brightnessManager)
    : mBrightnessManager(brightnessManager)
{
}

void BrightnessDemoModeHandler::demoStart()
{
}

void BrightnessDemoModeHandler::demoStop()
{
    mBrightnessManager.setBrightnessPercentage(100);
}
