/*
 * DefaultConvertModifierStrategy.cpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#include "DefaultConvertModifierStrategy.hpp"

LayoutModifiersDefs::LayoutModifierFlags DefaultConvertModifierStrategy::convertButtonToModifierFlags(
    ButtonID button)
{
    LayoutModifiersDefs::LayoutModifierFlags modifierFlags;

    if(isShiftL(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::SHIFT_LEFT, true);
    }
    if(isShiftR(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::SHIFT_RIGHT, true);
    }
    if(isAltL(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::ALT_LEFT, true);
    }
    if(isAltR(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::ALT_RIGHT, true);
    }
    if(isFn(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::FN, true);
    }
    if(isCtrlL(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::CTRL_LEFT, true);
    }
    if(isCtrlR(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::CTRL_RIGHT, true);
    }
    if(isGUIL(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::GUI_LEFT, true);
    }
    if(isGUIR(button))
    {
        modifierFlags.set(LayoutModifiersDefs::Flag::GUI_RIGHT, true);
    }

    return modifierFlags;
}

bool DefaultConvertModifierStrategy::isCtrlR(ButtonID button)
{
    return ButtonID(BTN_CTRL_R_MAC_ALT_R) == button;
}

bool DefaultConvertModifierStrategy::isCtrlL(ButtonID button)
{
    return ButtonID(BTN_CTRL_L_MAC_FN) == button;
}

bool DefaultConvertModifierStrategy::isAltR(ButtonID button)
{
    return ButtonID(BTN_ALT_R_MAC_GUI_R) == button;
}

bool DefaultConvertModifierStrategy::isAltL(ButtonID button)
{
    return ButtonID(BTN_ALT_L_MAC_GUI_L) == button;
}

bool DefaultConvertModifierStrategy::isShiftR(ButtonID button)
{
    return ButtonID(BTN_SHIFT_R) == button;
}

bool DefaultConvertModifierStrategy::isFn(ButtonID button)
{
    return ButtonID(BTN_FN_MAC_CTRL_L) == button;
}

bool DefaultConvertModifierStrategy::isShiftL(ButtonID button)
{
    return ButtonID(BTN_SHIFT_L) == button;
}

bool DefaultConvertModifierStrategy::isGUIR(ButtonID button)
{
    return false;
}

bool DefaultConvertModifierStrategy::isGUIL(ButtonID button)
{
    return ButtonID(BTN_GUI_L_MAC_ALT_L) == button;
}
