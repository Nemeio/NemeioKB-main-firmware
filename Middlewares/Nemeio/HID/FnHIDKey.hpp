/*
 * FnHIDKey.hpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_FNHIDKEY_HPP_
#define NEMEIO_HID_FNHIDKEY_HPP_

#include "AbstractContextualHIDKey.hpp"

class FnHIDKey : public AbstractContextualHIDKey
{
public:
    FnHIDKey(Keycode defaultKeycode, Keycode fnKeycode);
    virtual ~FnHIDKey() = default;

private:
    const Keycode mFnKeyCode;

    Keycode getKeycodeFromContext(const KeyboardContext& kbdContext);
};

#endif /* NEMEIO_HID_FNHIDKEY_HPP_ */
