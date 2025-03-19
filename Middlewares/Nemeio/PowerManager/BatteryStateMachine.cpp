/*
 * BatteryStateMachine.cpp
 *
 *  Created on: 17 avr. 2020
 *      Author: eskoric
 */

#include "BatteryStateMachine.hpp"

BatteryStateMachine::BatteryStateMachine(IBatteryStateListener& batteryStateListener)
    : mBatteryStateListener(batteryStateListener)
    , mState(BatteryState::INIT)
    , mLowBatteryLevelThresholdPercent(DEFAULT_LOW_BATTERY_THRESHOLD_PERCENT)
    , mBatteryLevelPercent(100)
{
}

void BatteryStateMachine::batteryLevel(uint8_t batteryLevelPercent)
{
    mBatteryLevelPercent = batteryLevelPercent;

    switch(mState)
    {
    case BatteryState::LOW:
        if(!isBatteryLow())
        {
            enterNormalBatteryState();
        }
        break;
    case BatteryState::NORMAL:
        if(isBatteryLow())
        {
            enterLowBatteryState();
        }
        break;
    case BatteryState::INIT:
    case BatteryState::CHARGING:
    default:
        break;
    }
}

void BatteryStateMachine::usbPluggedInComputer()
{
    pluggedIn();
}

void BatteryStateMachine::usbPluggedInComputerSuspended()
{
    pluggedIn();
}

void BatteryStateMachine::usbPluggedInPowerSupplyOnly()
{
    pluggedIn();
}

void BatteryStateMachine::usbPluggedOut()
{
    pluggedOut();
}

void BatteryStateMachine::pluggedIn()
{
    switch(mState)
    {
    case BatteryState::INIT:
    case BatteryState::LOW:
    case BatteryState::NORMAL:
        enterChargingState();
        break;
    case BatteryState::CHARGING:
    default:
        break;
    }
}

void BatteryStateMachine::pluggedOut()
{
    switch(mState)
    {
    case BatteryState::INIT:
    case BatteryState::CHARGING:
        if(isBatteryLow())
        {
            enterLowBatteryState();
        }
        else
        {
            enterNormalBatteryState();
        }
        break;
    case BatteryState::LOW:
    case BatteryState::NORMAL:
    default:
        break;
    }
}

void BatteryStateMachine::enterChargingState()
{
    mState = BatteryState::CHARGING;
    mBatteryStateListener.charging();
}

void BatteryStateMachine::enterLowBatteryState()
{
    mState = BatteryState::LOW;
    mBatteryStateListener.lowBatteryState();
}

bool BatteryStateMachine::isBatteryLow()
{
    return mBatteryLevelPercent <= mLowBatteryLevelThresholdPercent;
}

void BatteryStateMachine::setLowBatteryLevelThresholdPercent(uint8_t lowBatteryLevelThresholdPercent)
{
    mLowBatteryLevelThresholdPercent = lowBatteryLevelThresholdPercent;
}

void BatteryStateMachine::enterNormalBatteryState()
{
    mState = BatteryState::NORMAL;
    mBatteryStateListener.normalBatteryState();
}
