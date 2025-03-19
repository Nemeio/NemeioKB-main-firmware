/*
 * AbstractContextualHIDKey.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#include "AbstractContextualHIDKey.hpp"

AbstractContextualHIDKey::AbstractContextualHIDKey(Keycode keycode)
    : HIDKey(keycode)
{
}

HIDKey::Keycode AbstractContextualHIDKey::pressedKey(const KeyboardContext& kbdContext)
{
    mCurrentKeycode = getKeycodeFromContext(kbdContext);
    return mCurrentKeycode;
}

HIDKey::Keycode AbstractContextualHIDKey::unpressedKey()
{
    Keycode keycode = mCurrentKeycode;
    mCurrentKeycode = Keycode::null;
    return keycode;
}
