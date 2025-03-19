/*
 * UpgradeDisplayState.hpp
 *
 *  Created on: 27 juil. 2020
 *      Author: eskoric
 */

#ifndef DISPLAY_DISPLAYSTATE_UPGRADEDISPLAYSTATE_HPP_
#define DISPLAY_DISPLAYSTATE_UPGRADEDISPLAYSTATE_HPP_

#include "KeyboardDisplayState.hpp"
#include "ApplicationConfig.h"
#include "Signal.hpp"
#include "IScreen.hpp"

template<std::size_t READ_FILE_BUFFER>
class UpgradeDisplayState : public KeyboardDisplayState
{
public:
    UpgradeDisplayState() = default;
    UpgradeDisplayState(uint8_t upgradeCounter, Signal& upgradeSignal);

    uint8_t apply(IScreen& screen, uint8_t fbNum) final;

    bool operator==(const UpgradeDisplayState& state) const;

private:
    uint8_t mUpgradeCounter = 0;
    Signal* mUpgradeSignal = nullptr;
};

template<std::size_t READ_FILE_BUFFER>
UpgradeDisplayState<READ_FILE_BUFFER>::UpgradeDisplayState(uint8_t upgradeCounter,
                                                           Signal& upgradeSignal)
    : mUpgradeCounter(upgradeCounter)
    , mUpgradeSignal(&upgradeSignal)
{
}

template<std::size_t READ_FILE_BUFFER>
uint8_t UpgradeDisplayState<READ_FILE_BUFFER>::apply(IScreen& screen, uint8_t fbNum)
{
    if(mUpgradeSignal != nullptr)
    {
        mUpgradeSignal->wait(DelayMs::INFINITE_DELAY);
    }

    return fbNum;
}

template<std::size_t READ_FILE_BUFFER>
bool UpgradeDisplayState<READ_FILE_BUFFER>::operator==(const UpgradeDisplayState& state) const
{
    return mUpgradeCounter == state.mUpgradeCounter;
}

#endif /* DISPLAY_DISPLAYSTATE_UPGRADEDISPLAYSTATE_HPP_ */
