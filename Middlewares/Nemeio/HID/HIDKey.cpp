/*
 * HIDKey.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#include "HIDKey.hpp"

HIDKey::HIDKey(Keycode keycode)
    : mKeycode(keycode)
{
}

HIDKey::Keycode HIDKey::pressedKey(const KeyboardContext& kbdContext)
{
    return mKeycode;
}

HIDKey::Keycode HIDKey::unpressedKey()
{
    return mKeycode;
}
