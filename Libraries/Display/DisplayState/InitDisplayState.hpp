/*
 * InitDisplayState.hpp
 *
 *  Created on: Sep 24, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_INITDISPLAYSTATE_HPP_
#define DISPLAY_DISPLAYSTATE_INITDISPLAYSTATE_HPP_

#include "KeyboardDisplayState.hpp"

class InitDisplayState : public KeyboardDisplayState
{
public:
    enum class InitDisplayStateValue
    {
        OFF,
        INITIALIZED
    };

    InitDisplayState();
    InitDisplayState(InitDisplayStateValue state);
    virtual ~InitDisplayState() = default;

    uint8_t apply(IScreen& screen, uint8_t fbNum) final;

    bool operator==(const InitDisplayState& state) const;

    bool isInitialized() const;

private:
    InitDisplayStateValue mState;
};

#endif /* DISPLAY_DISPLAYSTATE_INITDISPLAYSTATE_HPP_ */
