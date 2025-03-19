/*
 * IScreen.hpp
 *
 *  Created on: Nov 4, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_ABSTRACTSCREEN_ISCREEN_HPP_
#define DISPLAY_SCREEN_ABSTRACTSCREEN_ISCREEN_HPP_

#include "ScreenDefs.hpp"
#include "IDataReader.hpp"
#include "IScreenController.hpp"

class IScreen
{
public:
    IScreen() = default;
    virtual ~IScreen() = default;

    virtual ScreenDefs::ScreenErrCode convertCoordinates(const ScreenDefs::Window& inputWindow,
                                                         ScreenDefs::Window& outputWindow) = 0;
    virtual const ScreenDefs::ScreenInfo getScreenInfo() const = 0;
    virtual IScreenController& getAssociatedScreenController() = 0;
};

#endif /* DISPLAY_SCREEN_ABSTRACTSCREEN_ISCREEN_HPP_ */
