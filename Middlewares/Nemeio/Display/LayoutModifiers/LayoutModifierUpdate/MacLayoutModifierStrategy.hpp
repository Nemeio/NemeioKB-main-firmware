/*
 * MacLayoutModifierStrategy.hpp
 *
 *  Created on: 2 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_MACLAYOUTMODIFIERSTRATEGY_HPP_
#define NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_MACLAYOUTMODIFIERSTRATEGY_HPP_

#include "DefaultLayoutModifierStrategy.hpp"

class MacLayoutModifierStrategy : public DefaultLayoutModifierStrategy
{
public:
    explicit MacLayoutModifierStrategy();
    virtual ~MacLayoutModifierStrategy() = default;

private:
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt MAC_ALT_GR_FLAGS =
        LayoutModifiersDefs::Flag::ALT_RIGHT | LayoutModifiersDefs::Flag::ALT_LEFT;
    static constexpr LayoutModifiersDefs::LayoutModifierStateFlagsInt
        MAC_ALLOWED_ALT_GR_COMBO_FLAGS = MAC_ALT_GR_FLAGS;

    bool isAltPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;
    bool isAltGrPressed(const LayoutModifiersDefs::LayoutModifierFlags flags) const;

    virtual LayoutModifiersDefs::LayoutModifierStateFlagsInt getAllowedAltGrComboFlags() const;
};

#endif /* NEMEIO_DISPLAY_LAYOUTMODIFIERUPDATE_MACLAYOUTMODIFIERSTRATEGY_HPP_ */
