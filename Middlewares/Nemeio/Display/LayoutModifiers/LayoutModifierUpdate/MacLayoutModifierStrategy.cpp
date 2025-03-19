/*
 * MacLayoutModifierStrategy.cpp
 *
 *  Created on: 2 juin 2020
 *      Author: eskoric
 */

#include "MacLayoutModifierStrategy.hpp"

bool MacLayoutModifierStrategy::isAltPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return false;
}

bool MacLayoutModifierStrategy::isAltGrPressed(
    const LayoutModifiersDefs::LayoutModifierFlags flags) const
{
    return flags.findAsLeastOneFlag(MAC_ALT_GR_FLAGS);
}

MacLayoutModifierStrategy::MacLayoutModifierStrategy()
    : DefaultLayoutModifierStrategy()
{
}

LayoutModifiersDefs::LayoutModifierStateFlagsInt
MacLayoutModifierStrategy::getAllowedAltGrComboFlags() const
{
    return MAC_ALLOWED_ALT_GR_COMBO_FLAGS;
}
