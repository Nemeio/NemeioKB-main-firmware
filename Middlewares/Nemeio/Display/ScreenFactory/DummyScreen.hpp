/*
 * DummyScreen.hpp
 *
 *  Created on: 2 nov. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_SCREENFACTORY_DUMMYSCREEN_HPP_
#define NEMEIO_DISPLAY_SCREENFACTORY_DUMMYSCREEN_HPP_

#include "IScreen.hpp"
#include "DummyScreenController.hpp"

class DummyScreen : public IScreen
{
public:
    explicit DummyScreen();
    explicit DummyScreen(IScreenController& screenController);
    explicit DummyScreen(ScreenDefs::ScreenInfo& screenInfos);
    virtual ~DummyScreen() = default;

    ScreenDefs::ScreenErrCode convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                 ScreenDefs::Window& outputWindow) final;
    const ScreenDefs::ScreenInfo getScreenInfo() const final;
    IScreenController& getAssociatedScreenController() final;

private:
    DummyScreenController mDummyScreenController;
    IScreenController& mScreenController;
    ScreenDefs::ScreenInfo mInfo = {0, 0};
};

#endif /* NEMEIO_DISPLAY_SCREENFACTORY_DUMMYSCREEN_HPP_ */
