/*
 * DeinitDisplayState.hpp
 *
 *  Created on: Sep 26, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_DISPLAYSTATE_DEINITDISPLAYSTATE_HPP_
#define DISPLAY_DISPLAYSTATE_DEINITDISPLAYSTATE_HPP_

#include "KeyboardDisplayState.hpp"
#include "DeinitDisplayStateListener.hpp"

class DeinitDisplayState : public KeyboardDisplayState
{
public:
    enum class DeinitDisplayStateValue
    {
        ON,
        DEINITIALIZED
    };

    DeinitDisplayState();
    DeinitDisplayState(DeinitDisplayStateListener& listener, DeinitDisplayStateValue state);
    virtual ~DeinitDisplayState() = default;

    uint8_t apply(IScreen& screen, uint8_t fbNum) final;

    bool operator==(const DeinitDisplayState& state) const;

private:
    DeinitDisplayStateValue mState;
    DeinitDisplayStateListener* mpListener;
};

#endif /* DISPLAY_DISPLAYSTATE_DEINITDISPLAYSTATE_HPP_ */
