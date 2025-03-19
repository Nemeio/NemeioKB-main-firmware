/*
 * ScreenFactory.cpp
 *
 *  Created on: 19 oct. 2021
 *      Author: thomas
 */

#include "ScreenFactory.hpp"

etl::optional<etl::reference_wrapper<IScreen>> ScreenFactory::makeScreen(
    IScreenController& screenController, ScreenDefs::ScreenInfo info)
{
    etl::optional<etl::reference_wrapper<IScreen>> screen = etl::nullopt;

    if(HolitechScreen::mScreenInfo == info)
    {
        mScreen.emplace<HolitechScreen>(screenController);
        screen.emplace(etl::get<HolitechScreen>(mScreen));
    }
    else if(EinkScreen::mScreenInfo == info)
    {
        mScreen.emplace<EinkScreen>(screenController);
        screen.emplace(etl::get<EinkScreen>(mScreen));
    }
    else
    {
        mScreen.emplace<DummyScreen>(mDummyScreenController);
        screen.emplace(etl::get<DummyScreen>(mScreen));
    }

    return screen;
}
