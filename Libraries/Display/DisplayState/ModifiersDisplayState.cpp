/*
 * ModifiersDisplayState.cpp
 *
 *  Created on: Mar 12, 2020
 *      Author: Labo
 */

#include "ModifiersDisplayState.hpp"

ModifiersDisplayState::ModifiersDisplayState(const ConfigurationDisplayInfo& configurationDisplayInfo,
                                             const LayoutModifiersDefs::LayoutModifierFlags& flags,
                                             bool highQualityModifier)
    : mFlags(flags)
    , mHighQualityModifier(highQualityModifier)
{
    if(configurationDisplayInfo.layout.has_value())
    {
        mBackgroundColor = configurationDisplayInfo.layout.value().getMetadata().background;
    }
}

uint8_t ModifiersDisplayState::apply(IScreen& screen,
                                     uint8_t fbNum,
                                     const ILayoutModifierStrategy& strategy)
{
    uint8_t newFbNum = fbNum;

    strategy.updateModifiers(
        mFlags,
        [&](uint8_t selectedFbNum)
        {
            IScreenController::DisplayOptions displayOptions{eDisplayMode::FULL, mBackgroundColor};
            displayOptions.displayMode = mHighQualityModifier ? eDisplayMode::FULL
                                                              : eDisplayMode::FAST;
            if(ScreenDefs::ScreenErrCode::SUCCESS
               == screen.getAssociatedScreenController().display(screen,
                                                                 selectedFbNum,
                                                                 displayOptions))
            {
                newFbNum = selectedFbNum;
            }
        });

    return newFbNum;
}

bool ModifiersDisplayState::equals(const ModifiersDisplayState& state,
                                   const ILayoutModifierStrategy& strategy) const
{
    return (strategy.convertFlagsToFbNum(mFlags) == strategy.convertFlagsToFbNum(state.mFlags));
}
