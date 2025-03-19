/*
 * KeyboardContext.hpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_KEYBOARDCONTEXT_HPP_
#define NEMEIO_HID_KEYBOARDCONTEXT_HPP_

class KeyboardContext
{
public:
    KeyboardContext();
    virtual ~KeyboardContext() = default;

    [[nodiscard]] bool isFnKeyPressed() const;
    void setFnKeyPressed(bool fnKeyPressed);
    [[nodiscard]] bool isMacConfig() const;
    void setMacConfig(bool macConfig);
    void clear();

private:
    bool bFnKeyPressed;
    bool bMacConfig;
};

inline bool KeyboardContext::isFnKeyPressed() const
{
    return bFnKeyPressed;
}

inline void KeyboardContext::setFnKeyPressed(bool fnKeyPressed)
{
    bFnKeyPressed = fnKeyPressed;
}

inline bool KeyboardContext::isMacConfig() const
{
    return bMacConfig;
}

inline KeyboardContext::KeyboardContext()
    : bFnKeyPressed(false)
    , bMacConfig(false)
{
}

inline void KeyboardContext::setMacConfig(bool macConfig)
{
    bMacConfig = macConfig;
}

inline void KeyboardContext::clear()
{
    bFnKeyPressed = false;
    bMacConfig = false;
}

#endif /* NEMEIO_HID_KEYBOARDCONTEXT_HPP_ */
