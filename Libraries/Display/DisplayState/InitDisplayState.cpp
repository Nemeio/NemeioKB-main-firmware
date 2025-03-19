/*
 * InitDisplayState.cpp
 *
 *  Created on: Sep 24, 2019
 *      Author: Labo
 */

#include "InitDisplayState.hpp"
#include "assertTools.h"

InitDisplayState::InitDisplayState()
    : InitDisplayState(InitDisplayStateValue::OFF)
{
}

InitDisplayState::InitDisplayState(InitDisplayStateValue state)
    : mState(state)
{
}

uint8_t InitDisplayState::apply(IScreen& screen, uint8_t fbNum)
{
    switch(mState)
    {
    case InitDisplayStateValue::INITIALIZED:
        break;
    case InitDisplayStateValue::OFF:
        break;
    default:
        ASSERT(false);
        break;
    }

    return fbNum;
}

bool InitDisplayState::operator==(const InitDisplayState& state) const
{
    return state.mState == mState;
}

bool InitDisplayState::isInitialized() const
{
    return InitDisplayStateValue::INITIALIZED == mState;
}
