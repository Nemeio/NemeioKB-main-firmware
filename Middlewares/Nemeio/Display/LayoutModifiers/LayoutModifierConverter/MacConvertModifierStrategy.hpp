/*
 * MacConvertModifierStrategy.hpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_MACCONVERTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_MACCONVERTMODIFIERSTRATEGY_HPP_

#include "DefaultConvertModifierStrategy.hpp"

class MacConvertModifierStrategy : public DefaultConvertModifierStrategy
{
public:
    MacConvertModifierStrategy() = default;
    virtual ~MacConvertModifierStrategy() = default;

    bool isCtrlR(ButtonID button);
    bool isCtrlL(ButtonID button);
    bool isAltR(ButtonID button);
    bool isAltL(ButtonID button);
    bool isFn(ButtonID button);
    bool isGUIR(ButtonID button);
    bool isGUIL(ButtonID button);
};

#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERCONVERTER_MACCONVERTMODIFIERSTRATEGY_HPP_ */
