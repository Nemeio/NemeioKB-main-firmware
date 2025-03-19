/*
 * AbstractKeyboardDisplayStatus.hpp
 *
 *  Created on: 3 juin 2020
 *      Author: eskoric
 */

#ifndef DISPLAY_DISPLAYSTATE_ABSTRACTKEYBOARDDISPLAYSTATUS_HPP_
#define DISPLAY_DISPLAYSTATE_ABSTRACTKEYBOARDDISPLAYSTATUS_HPP_

#include "IScreen.hpp"

template<class DisplayState>
class AbstractKeyboardDisplayStatus
{
public:
    AbstractKeyboardDisplayStatus() = default;
    virtual ~AbstractKeyboardDisplayStatus() = default;

    bool update(IScreen& screen, uint8_t& fbNum);

    void prepareTargetStateCopy();
    void setTargetState(const DisplayState& state);
    void setCurrentState(const DisplayState& state);
    DisplayState getTargetState() const;

protected:
    DisplayState mCurrentState;

private:
    DisplayState mTargetState;
    DisplayState mTargetStateCopy;

    virtual bool isUpToDate(const DisplayState& currentState,
                            const DisplayState& targetStateCopy) = 0;
    virtual uint8_t updateTargetState(DisplayState& targetStateCopy,
                                      IScreen& screen,
                                      uint8_t fbNum) = 0;
};

template<class DisplayState>
void AbstractKeyboardDisplayStatus<DisplayState>::setTargetState(const DisplayState& state)
{
    mTargetState = state;
}

template<class DisplayState>
void AbstractKeyboardDisplayStatus<DisplayState>::setCurrentState(const DisplayState& state)
{
    mCurrentState = state;
}

template<class DisplayState>
inline void AbstractKeyboardDisplayStatus<DisplayState>::prepareTargetStateCopy()
{
    mTargetStateCopy = mTargetState;
}

template<class DisplayState>
DisplayState AbstractKeyboardDisplayStatus<DisplayState>::getTargetState() const
{
    return mTargetState;
}

template<class DisplayState>
bool AbstractKeyboardDisplayStatus<DisplayState>::update(IScreen& screen, uint8_t& fbNum)
{
    bool bNeedUpdate = false;
    DisplayState targetStateCopy;

    bNeedUpdate = !isUpToDate(mCurrentState, mTargetStateCopy);
    mCurrentState = mTargetStateCopy;

    if(bNeedUpdate)
    {
        updateTargetState(mTargetStateCopy, screen, fbNum);
    }

    return bNeedUpdate;
}

#endif /* DISPLAY_DISPLAYSTATE_ABSTRACTKEYBOARDDISPLAYSTATUS_HPP_ */
