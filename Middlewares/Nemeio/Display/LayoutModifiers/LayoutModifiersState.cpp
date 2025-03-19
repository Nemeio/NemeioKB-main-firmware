/*
 * KeypadModifierConverter.cpp
 *
 *  Created on: 29 mai 2020
 *      Author: eskoric
 */

#include "LayoutModifiersState.hpp"
#include <atomic>
#include <string>
#include "DefaultConvertModifierStrategy.hpp"
#include "MacConvertModifierStrategy.hpp"
#include "AutoLock.h"

LayoutModifiersState::LayoutModifiersState(AbstractRecursiveMutex& mutex,
                                           ILayoutModifierListener& listener)
    : mStateMutex(mutex)
    , mListener(listener)
{
}

void LayoutModifiersState::keyLockStatesChanged(const Identifiable& source,
                                                const HIDDefs::KeyLockStates& keyLockStates)
{
    updateAndNotifyState(LayoutModifiersDefs::Flag::CAPSLOCK, keyLockStates.capsLock);
}

void LayoutModifiersState::selectTouch(const ButtonID& button,
                                       bool bPressed,
                                       const Configuration& config)
{
    LayoutModifiersDefs::LayoutModifierFlags newModifiersFlags;

    if(config.getOperatingSystem() != configuration::OperatingSystem::MAC)
    {
        DefaultConvertModifierStrategy converter;
        newModifiersFlags = converter.convertButtonToModifierFlags(button);
    }
    else
    {
        MacConvertModifierStrategy converter;
        newModifiersFlags = converter.convertButtonToModifierFlags(button);
    }

    updateAndNotifyState(newModifiersFlags, bPressed);
}

void LayoutModifiersState::notifyState(const LayoutModifiersDefs::LayoutModifierFlags& state)
{
    mListener.layoutModifiersStateChanged(state);
}

void LayoutModifiersState::updateAndNotifyState(LayoutModifiersDefs::LayoutModifierFlags flags,
                                                bool bPressed)
{
    LayoutModifiersDefs::LayoutModifierFlags stateCopy;
    bool bNotify = false;

    {
        AutoLock lock(mStateMutex);

        stateCopy = mState;

        mState.set(flags, bPressed);

        if(stateCopy != mState)
        {
            stateCopy = mState;
            bNotify = true;
        }
    }

    if(bNotify)
    {
        notifyState(stateCopy);
    }
}

void LayoutModifiersState::pressedKeyWithConfig(const ButtonID& button, const Configuration& config)
{
    selectTouch(button, true, config);
}

void LayoutModifiersState::unpressedKeyWithConfig(const ButtonID& button,
                                                  const Configuration& config)
{
    selectTouch(button, false, config);
}

void LayoutModifiersState::terminateUpdateKeysWithConfig(const Configuration& config)
{
    // Nothing to do
}

void LayoutModifiersState::resetKeysWithConfig(const Configuration& config)
{
    // Keep CAPSLOCK flag value and clear the others
    LayoutModifiersDefs::LayoutModifierFlags resetValue(~LayoutModifiersDefs::Flag::CAPSLOCK);
    updateAndNotifyState(resetValue, false);
}