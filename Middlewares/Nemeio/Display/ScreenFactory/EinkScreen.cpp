/*
 * EinkScreen.cpp
 *
 *  Created on: 19 oct. 2021
 *      Author: thomas
 */

#include "EinkScreen.hpp"

EinkScreen::EinkScreen(IScreenController& screenController)
    : mScreenController(screenController)
{
}

ScreenDefs::ScreenErrCode EinkScreen::convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                         ScreenDefs::Window& outputWindow)
{
    outputWindow = inputWindow;

    return ScreenDefs::ScreenErrCode::SUCCESS;
}

const ScreenDefs::ScreenInfo EinkScreen::getScreenInfo() const
{
    return mScreenInfo;
}

IScreenController& EinkScreen::getAssociatedScreenController()
{
    return mScreenController;
}
