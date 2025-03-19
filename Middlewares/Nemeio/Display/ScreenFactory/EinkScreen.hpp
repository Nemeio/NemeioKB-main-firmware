/*
 * EinkScreen.hpp
 *
 *  Created on: 19 oct. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_EINKSCREEN_HPP_
#define NEMEIO_DISPLAY_EINKSCREEN_HPP_

#include "IScreen.hpp"

class EinkScreen : public IScreen
{
public:
    explicit EinkScreen(IScreenController& screenController);
    virtual ~EinkScreen() = default;

    ScreenDefs::ScreenErrCode convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                 ScreenDefs::Window& outputWindow) final;
    const ScreenDefs::ScreenInfo getScreenInfo() const final;
    IScreenController& getAssociatedScreenController() final;

    static constexpr ScreenDefs::ScreenInfo mScreenInfo = {.width = 2400, .height = 1034};

private:
    IScreenController& mScreenController;
};

#endif /* NEMEIO_DISPLAY_EINKSCREEN_HPP_ */
