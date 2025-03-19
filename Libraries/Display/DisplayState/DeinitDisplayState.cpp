/*
 * DeinitDisplayState.cpp
 *
 *  Created on: Sep 26, 2019
 *      Author: Labo
 */

#include "DeinitDisplayState.hpp"
#include "assertTools.h"

DeinitDisplayState::DeinitDisplayState()
    : mState(DeinitDisplayStateValue::ON)
    , mpListener(nullptr)
{
}

DeinitDisplayState::DeinitDisplayState(DeinitDisplayStateListener& listener,
                                       DeinitDisplayStateValue state)
    : mState(state)
    , mpListener(&listener)
{
}

uint8_t DeinitDisplayState::apply(IScreen& screen, uint8_t fbNum)
{
    switch(mState)
    {
    case DeinitDisplayStateValue::ON:
        break;
    case DeinitDisplayStateValue::DEINITIALIZED:
        screen.getAssociatedScreenController().setPower(false);
        if(mpListener != nullptr)
        {
            mpListener->displayDeinited();
        }
        break;
    default:
        ASSERT(false);
        break;
    }

    return fbNum;
}

bool DeinitDisplayState::operator==(const DeinitDisplayState& state) const
{
    return mState == state.mState;
}
