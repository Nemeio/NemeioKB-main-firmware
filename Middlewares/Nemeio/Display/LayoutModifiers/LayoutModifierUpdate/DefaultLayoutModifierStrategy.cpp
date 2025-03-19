/*
 * DefaultLayoutModifierStrategy.cpp
 *
 *  Created on: 2 juin 2020
 *      Author: eskoric
 */

#include "DefaultLayoutModifierStrategy.hpp"

void DefaultLayoutModifierStrategy::updateModifiers(
    const LayoutModifiersDefs::LayoutModifierFlags& flags,
    const std::function<void(uint8_t)>& displayCallback) const
{
    if(displayCallback)
    {
        displayCallback(static_cast<uint8_t>(convertFlagsToFbNum(flags)));
    }
}

LayoutModifiersDefs::FrameBufferIndex DefaultLayoutModifierStrategy::convertFlagsToFbNum(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    LayoutModifiersDefs::FrameBufferIndex fb = LayoutModifiersDefs::FrameBufferIndex::DEFAULT;

    if(isCapslockCombo(flags))
    {
        fb = LayoutModifiersDefs::FrameBufferIndex::CAPSLOCK;
    }
    else if(isShiftCombo(flags))
    {
        fb = LayoutModifiersDefs::FrameBufferIndex::SHIFT;
    }
    else if(isAltGrCombo(flags))
    {
        fb = LayoutModifiersDefs::FrameBufferIndex::ALTGR;
    }
    else if(isShiftAltGrCombo(flags))
    {
        fb = LayoutModifiersDefs::FrameBufferIndex::SHIFT_ALTGR;
    }
    else if(isFnCombo(flags))
    {
        fb = LayoutModifiersDefs::FrameBufferIndex::FN;
    }

    return fb;
}

bool DefaultLayoutModifierStrategy::isShiftPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(SHIFT_FLAGS);
}

bool DefaultLayoutModifierStrategy::isCtrlPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(CTRL_FLAGS);
}

bool DefaultLayoutModifierStrategy::isAltPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(ALT_LEFT_FLAGS);
}

bool DefaultLayoutModifierStrategy::isAltGrPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(ALT_RIGHT_FLAGS)
           || (isCtrlPressed(flags) && isAltPressed(flags));
}

bool DefaultLayoutModifierStrategy::isCapslockPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(CAPSLOCK_FLAGS);
}

bool DefaultLayoutModifierStrategy::isFnPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(FN_FLAGS);
}

bool DefaultLayoutModifierStrategy::isGuiPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(GUI_FLAGS);
}

bool DefaultLayoutModifierStrategy::isShiftCombo(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return isShiftPressed(flags)
           && LayoutModifiersDefs::LayoutModifierFlags(ALLOWED_SHIFT_COMBO_FLAGS).findAllFlags(flags);
}

bool DefaultLayoutModifierStrategy::isAltGrCombo(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return isAltGrPressed(flags)
           && LayoutModifiersDefs::LayoutModifierFlags(getAllowedAltGrComboFlags())
                  .findAllFlags(flags);
}

bool DefaultLayoutModifierStrategy::isShiftAltGrCombo(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return isShiftPressed(flags) && isAltGrPressed(flags)
           && LayoutModifiersDefs::LayoutModifierFlags(getAllowedAltGrComboFlags()
                                                       | ALLOWED_SHIFT_COMBO_FLAGS)
                  .findAllFlags(flags);
}

bool DefaultLayoutModifierStrategy::isCapslockCombo(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return isCapslockPressed(flags)
           && LayoutModifiersDefs::LayoutModifierFlags(ALLOWED_CAPSLOCK_COMBO_FLAGS)
                  .findAllFlags(flags);
}
bool DefaultLayoutModifierStrategy::isFnCombo(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return isFnPressed(flags)
           && LayoutModifiersDefs::LayoutModifierFlags(ALLOWED_FN_COMBO_FLAGS).findAllFlags(flags);
}

uint8_t DefaultLayoutModifierStrategy::getNbImages() const
{
    return NB_MANAGED_COMBINATIONS;
}

LayoutModifiersDefs::LayoutModifierStateFlagsInt
DefaultLayoutModifierStrategy::getAllowedAltGrComboFlags() const
{
    return ALLOWED_ALT_GR_COMBO_FLAGS;
}
