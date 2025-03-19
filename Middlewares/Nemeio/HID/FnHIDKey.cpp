/*
 * FnHIDKey.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#include "FnHIDKey.hpp"

FnHIDKey::FnHIDKey(Keycode defaultKeycode, Keycode fnKeycode)
    : AbstractContextualHIDKey(defaultKeycode)
    , mFnKeyCode(fnKeycode)
{
}

FnHIDKey::Keycode FnHIDKey::getKeycodeFromContext(const KeyboardContext& kbdContext)
{
    Keycode keycode = mKeycode;

    if(kbdContext.isFnKeyPressed())
    {
        keycode = mFnKeyCode;
    }

    return keycode;
}
