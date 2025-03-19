/*
 * HolitechScreen.hpp
 *
 *  Created on: 19 oct. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_HOLITECHSCREEN_HPP_
#define NEMEIO_DISPLAY_HOLITECHSCREEN_HPP_

#include "IScreen.hpp"

class HolitechScreen : public IScreen
{
public:
    explicit HolitechScreen(IScreenController& screenController);
    virtual ~HolitechScreen() = default;

    ScreenDefs::ScreenErrCode convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                 ScreenDefs::Window& outputWindow) final;
    const ScreenDefs::ScreenInfo getScreenInfo() const final;
    IScreenController& getAssociatedScreenController() final;

    static constexpr ScreenDefs::ScreenInfo mScreenInfo = {.width = 1496, .height = 624};

private:
    IScreenController& mScreenController;
};

#endif /* NEMEIO_DISPLAY_HOLITECHSCREEN_HPP_ */
