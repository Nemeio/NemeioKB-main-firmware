/*
 * MacHIDKey.hpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_MACHIDKEY_HPP_
#define NEMEIO_HID_MACHIDKEY_HPP_

#include "AbstractContextualHIDKey.hpp"

class MacHIDKey : public AbstractContextualHIDKey
{
public:
    MacHIDKey(Keycode defaultKeycode, Keycode macKeycode);
    virtual ~MacHIDKey() = default;

private:
    const Keycode mMacKeyCode;

    Keycode getKeycodeFromContext(const KeyboardContext& kbdContext);
};

#endif /* NEMEIO_HID_MACHIDKEY_HPP_ */
