/*
 * DummyScreen.cpp
 *
 *  Created on: 2 nov. 2021
 *      Author: thomas
 */

#include <DummyScreen.hpp>

DummyScreen::DummyScreen()
    : mScreenController(mDummyScreenController)
{
}

DummyScreen::DummyScreen(IScreenController& screenController)
    : mScreenController(screenController)
{
}

DummyScreen::DummyScreen(ScreenDefs::ScreenInfo& screenInfos)
    : mScreenController(mDummyScreenController)
    , mInfo(screenInfos)
{
}

ScreenDefs::ScreenErrCode DummyScreen::convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                          ScreenDefs::Window& outputWindow)
{
    outputWindow = inputWindow;
    return ScreenDefs::ScreenErrCode::SUCCESS;
}

const ScreenDefs::ScreenInfo DummyScreen::getScreenInfo() const
{
    return mInfo;
}

IScreenController& DummyScreen::getAssociatedScreenController()
{
    return mScreenController;
}
