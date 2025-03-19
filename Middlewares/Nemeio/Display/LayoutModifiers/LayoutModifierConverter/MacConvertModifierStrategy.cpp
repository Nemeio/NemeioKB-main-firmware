/*
 * MacConvertModifierStrategy.cpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#include "MacConvertModifierStrategy.hpp"
#include "keyIDConfig.h"

bool MacConvertModifierStrategy::isCtrlR(ButtonID button)
{
    return false;
}

bool MacConvertModifierStrategy::isCtrlL(ButtonID button)
{
    return ButtonID(BTN_FN_MAC_CTRL_L) == button;
}

bool MacConvertModifierStrategy::isAltR(ButtonID button)
{
    return ButtonID(BTN_CTRL_R_MAC_ALT_R) == button;
}

bool MacConvertModifierStrategy::isAltL(ButtonID button)
{
    return ButtonID(BTN_GUI_L_MAC_ALT_L) == button;
}

bool MacConvertModifierStrategy::isFn(ButtonID button)
{
    return ButtonID(BTN_CTRL_L_MAC_FN) == button;
}

bool MacConvertModifierStrategy::isGUIR(ButtonID button)
{
    return ButtonID(BTN_ALT_R_MAC_GUI_R) == button;
}

bool MacConvertModifierStrategy::isGUIL(ButtonID button)
{
    return ButtonID(BTN_ALT_L_MAC_GUI_L) == button;
}
