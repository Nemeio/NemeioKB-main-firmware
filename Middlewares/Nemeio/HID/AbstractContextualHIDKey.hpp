/*
 * AbstractContextualHIDKey.hpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_ABSTRACTCONTEXTUALHIDKEY_HPP_
#define NEMEIO_HID_ABSTRACTCONTEXTUALHIDKEY_HPP_

#include "HIDKey.hpp"

class AbstractContextualHIDKey : public HIDKey
{
public:
    AbstractContextualHIDKey(Keycode keycode);
    virtual ~AbstractContextualHIDKey() = default;

    Keycode pressedKey(const KeyboardContext& kbdContext);
    Keycode unpressedKey();

private:
    Keycode mCurrentKeycode = Keycode::null;

    virtual Keycode getKeycodeFromContext(const KeyboardContext& kbdContext) = 0;
};

#endif /* NEMEIO_HID_ABSTRACTCONTEXTUALHIDKEY_HPP_ */
