/*
 * MacHIDKey.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#include "MacHIDKey.hpp"

MacHIDKey::MacHIDKey(Keycode defaultKeycode, Keycode macKeycode)
    : AbstractContextualHIDKey(defaultKeycode)
    , mMacKeyCode(macKeycode)
{
}

HIDKey::Keycode MacHIDKey::getKeycodeFromContext(const KeyboardContext& kbdContext)
{
    Keycode keycode = mKeycode;

    if(kbdContext.isMacConfig())
    {
        keycode = mMacKeyCode;
    }

    return keycode;
}
