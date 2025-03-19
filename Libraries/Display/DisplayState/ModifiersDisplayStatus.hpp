/*
 * ModifiersDisplayStatus.hpp
 *
 *  Created on: 3 juin 2020
 *      Author: eskoric
 */

#ifndef DISPLAY_DISPLAYSTATE_MODIFIERSDISPLAYSTATUS_HPP_
#define DISPLAY_DISPLAYSTATE_MODIFIERSDISPLAYSTATUS_HPP_

#include "AbstractKeyboardDisplayStatus.hpp"

class ModifiersDisplayStatus : public AbstractKeyboardDisplayStatus<ModifiersDisplayState>
{
public:
    ModifiersDisplayStatus() = default;
    virtual ~ModifiersDisplayStatus() = default;

    void setModifierStrategy(const ILayoutModifierStrategy& strategy);

private:
    const ILayoutModifierStrategy* mpModifierStrategy = nullptr;
    bool isUpToDate(const ModifiersDisplayState& currentState,
                    const ModifiersDisplayState& targetStateCopy);
    uint8_t updateTargetState(ModifiersDisplayState& targetStateCopy,
                              IScreen& screen,
                              uint8_t fbNum);
};

inline void ModifiersDisplayStatus::setModifierStrategy(const ILayoutModifierStrategy& strategy)
{
    mpModifierStrategy = &strategy;
}

inline bool ModifiersDisplayStatus::isUpToDate(const ModifiersDisplayState& currentState,
                                               const ModifiersDisplayState& targetStateCopy)
{
    bool bRet = false;

    if(mpModifierStrategy != nullptr)
    {
        bRet = targetStateCopy.equals(currentState, *mpModifierStrategy);
    }

    return bRet;
}

inline uint8_t ModifiersDisplayStatus::updateTargetState(ModifiersDisplayState& targetStateCopy,
                                                         IScreen& screen,
                                                         uint8_t fbNum)
{
    uint8_t newFbNum = fbNum;

    if(mpModifierStrategy != nullptr)
    {
        newFbNum = targetStateCopy.apply(screen, fbNum, *mpModifierStrategy);
    }

    return newFbNum;
}

#endif /* DISPLAY_DISPLAYSTATE_MODIFIERSDISPLAYSTATUS_HPP_ */
