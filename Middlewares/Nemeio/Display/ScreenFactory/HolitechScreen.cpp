/*
 * HolitechScreen.cpp
 *
 *  Created on: 19 oct. 2021
 *      Author: thomas
 */

#include "HolitechScreen.hpp"

HolitechScreen::HolitechScreen(IScreenController& screenController)
    : mScreenController(screenController)
{
}

ScreenDefs::ScreenErrCode HolitechScreen::convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                             ScreenDefs::Window& outputWindow)
{
    if(inputWindow.x + inputWindow.w > mScreenInfo.width
       || inputWindow.y + inputWindow.h > mScreenInfo.height)
    {
        return ScreenDefs::ScreenErrCode::INVALID_PARAMETER;
    }

    outputWindow.x = mScreenInfo.width - inputWindow.x - inputWindow.w;
    outputWindow.y = mScreenInfo.height - inputWindow.y - inputWindow.h;
    outputWindow.w = inputWindow.w;
    outputWindow.h = inputWindow.h;

    return ScreenDefs::ScreenErrCode::SUCCESS;
}

const ScreenDefs::ScreenInfo HolitechScreen::getScreenInfo() const
{
    return mScreenInfo;
}

IScreenController& HolitechScreen::getAssociatedScreenController()
{
    return mScreenController;
}
