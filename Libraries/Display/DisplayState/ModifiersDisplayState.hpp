/*
 * ModifiersDisplayState.hpp
 *
 *  Created on: Mar 12, 2020
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_MODIFIERSDISPLAYSTATE_HPP_
#define DISPLAY_DISPLAYSTATE_MODIFIERSDISPLAYSTATE_HPP_

#include "KeyboardDisplayState.hpp"
#include "LayoutModifiersDefs.hpp"
#include "ILayoutModifierStrategy.hpp"
#include "ConfigurationDisplayInfo.hpp"

class ModifiersDisplayState
{
public:
    ModifiersDisplayState() = default;
    virtual ~ModifiersDisplayState() = default;
    ModifiersDisplayState(const ConfigurationDisplayInfo& configurationDisplayInfo,
                          const LayoutModifiersDefs::LayoutModifierFlags& flags,
                          bool highQualityModifier);

    uint8_t apply(IScreen& screen, uint8_t fbNum, const ILayoutModifierStrategy& strategy);

    bool equals(const ModifiersDisplayState& state, const ILayoutModifierStrategy& strategy) const;

private:
    ScreenDefs::BackgroundColor mBackgroundColor{ScreenDefs::BackgroundColor::UNKNOWN};
    LayoutModifiersDefs::LayoutModifierFlags mFlags{0};
    bool mHighQualityModifier{false};
};

#endif /* DISPLAY_DISPLAYSTATE_MODIFIERSDISPLAYSTATE_HPP_ */
