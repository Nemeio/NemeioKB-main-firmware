/*
 * KeyboardDisplayStatus.hpp
 *
 *  Created on: Sep 23, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_KEYBOARDDISPLAYSTATUS_HPP_
#define DISPLAY_DISPLAYSTATE_KEYBOARDDISPLAYSTATUS_HPP_

#include "AbstractKeyboardDisplayStatus.hpp"

template<class DisplayState>
class KeyboardDisplayStatus : public AbstractKeyboardDisplayStatus<DisplayState>
{
public:
    KeyboardDisplayStatus() = default;
    virtual ~KeyboardDisplayStatus() = default;

private:
    virtual bool isUpToDate(const DisplayState& currentState, const DisplayState& targetStateCopy);
    virtual uint8_t updateTargetState(DisplayState& targetStateCopy, IScreen& screen, uint8_t fbNum);
};

template<class DisplayState>
bool KeyboardDisplayStatus<DisplayState>::isUpToDate(const DisplayState& currentState,
                                                     const DisplayState& targetStateCopy)
{
    return currentState == targetStateCopy;
}

template<class DisplayState>
uint8_t KeyboardDisplayStatus<DisplayState>::updateTargetState(DisplayState& targetStateCopy,
                                                               IScreen& screen,
                                                               uint8_t fbNum)
{
    return targetStateCopy.apply(screen, fbNum);
}

#endif /* DISPLAY_DISPLAYSTATE_KEYBOARDDISPLAYSTATUS_HPP_ */
