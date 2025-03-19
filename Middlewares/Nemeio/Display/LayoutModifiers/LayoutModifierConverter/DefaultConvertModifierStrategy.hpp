/*
 * DefaultConvertModifierStrategy.hpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_DEFAULTCONVERTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_DEFAULTCONVERTMODIFIERSTRATEGY_HPP_

#include "LayoutModifiersDefs.hpp"
#include "keyIDConfig.h"

class DefaultConvertModifierStrategy
{
public:
    DefaultConvertModifierStrategy() = default;
    virtual ~DefaultConvertModifierStrategy() = default;

    LayoutModifiersDefs::LayoutModifierFlags convertButtonToModifierFlags(ButtonID button);

private:
    virtual bool isCtrlR(ButtonID button);
    virtual bool isCtrlL(ButtonID button);
    virtual bool isAltR(ButtonID button);
    virtual bool isAltL(ButtonID button);
    virtual bool isFn(ButtonID button);
    bool isShiftR(ButtonID button);
    bool isShiftL(ButtonID button);
    virtual bool isGUIR(ButtonID button);
    virtual bool isGUIL(ButtonID button);
};

#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_DEFAULTCONVERTMODIFIERSTRATEGY_HPP_ */
