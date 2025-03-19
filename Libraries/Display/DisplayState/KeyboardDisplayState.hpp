/*
 * KeyboardDisplayState.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_KEYBOARDDISPLAYSTATE_HPP_
#define DISPLAY_KEYBOARDDISPLAYSTATE_HPP_

#include "IScreen.hpp"

class KeyboardDisplayState
{
public:
    KeyboardDisplayState() = default;
    virtual ~KeyboardDisplayState() = default;

    virtual uint8_t apply(IScreen& screen, uint8_t fbNum) = 0;
};

#endif /* DISPLAY_KEYBOARDDISPLAYSTATE_HPP_ */
