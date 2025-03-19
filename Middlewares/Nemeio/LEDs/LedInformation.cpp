/*
 * LedInformation.cpp
 *
 *  Created on: 4 mars 2021
 *      Author: thomas
 */

#include "LedInformation.hpp"
#include "ILED.hpp"
#include "AutoLock.h"
#include "etl/overload.h"

LedInformation::LedInformation(ILED& capsLock,
                               ILED& battery,
                               ILED& ble,
                               ITimer& batteryTimer,
                               ITimer& bleTimer,
                               ITimer& updateTimer)
    : mCapsLockLed(capsLock)
    , mBatteryLed(battery)
    , mBleLed(ble)
    , mBatteryLedTimer(batteryTimer)
    , mBleLedTimer(bleTimer)
    , mUpdateLedTimer(updateTimer)
    , mUpdateLedStrategy(etl::in_place_type<SynchronizedBlinkingLED>,
                         mBleLed,
                         mCapsLockLed,
                         mUpdateLedTimer)
    , mCapsLockLedStrategy(etl::in_place_type<StaticLED>, mCapsLockLed, staticLEDState::LED_OFF)
    , mBatteryLedStrategy(etl::in_place_type<StaticLED>, mBatteryLed, staticLEDState::LED_OFF)
    , mBleLedStrategy(etl::in_place_type<StaticLED>, mBleLed, staticLEDState::LED_OFF)
{
    mUpdateLedTimer.setDelay(mUpdateBlinkingDelayMs);
    mBatteryLedTimer.setDelay(mBleBlinkingDelayOnMs);
    mBleLedTimer.setDelay(mBleBlinkingDelayOnMs);
}


void LedInformation::startStrategy(ledStrategy& strategy) const
{
    auto visitor = etl::make_overload([](auto& arg) { arg.startLedStrategy(); });

    strategy.accept_functor(visitor);
}

void LedInformation::stopStrategy(ledStrategy& strategy) const
{
    auto visitor = etl::make_overload([](auto& arg) { arg.stopLedStrategy(); });

    strategy.accept_functor(visitor);
}


void LedInformation::setNewStrategy(ledStrategy& strategy, ledStrategy&& newStrategy)
{
    AutoLock autoLock(mMutex);

    if(!mUpdateInProgress)
    {
        stopStrategy(strategy);
    }

    strategy = std::move(newStrategy);

    if(!mUpdateInProgress)
    {
        startStrategy(strategy);
    }
}

void LedInformation::notification(BleEvent event)
{
    switch(event)
    {
    case BleEvent::ADVERTISING_STARTED:
        setNewStrategy(mBleLedStrategy,
                       BlinkingLED(mBleLed,
                                   mBleLedTimer,
                                   mBleBlinkingDelayOnMs,
                                   mBleBlinkingDelayOffMs));
        break;
    case BleEvent::ADVERTISING_STOPPED:
        setNewStrategy(mBleLedStrategy, StaticLED(mBleLed, staticLEDState::LED_OFF));
        break;
    case BleEvent::CONNECTED:
        setNewStrategy(mBleLedStrategy, StaticLED(mBleLed, staticLEDState::LED_ON));
        break;
    case BleEvent::DISCONNECTED:
        setNewStrategy(mBleLedStrategy, StaticLED(mBleLed, staticLEDState::LED_OFF));
        break;
    default:
        break;
    }
}

void LedInformation::lowBatteryState()
{
    setNewStrategy(mBatteryLedStrategy,
                   BlinkingLED(mBatteryLed,
                               mBatteryLedTimer,
                               mBatteryBlinkingDelayOnMs,
                               mBatteryBlinkingDelayOffMs));
}

void LedInformation::normalBatteryState()
{
    setNewStrategy(mBatteryLedStrategy, StaticLED(mBatteryLed, staticLEDState::LED_OFF));
}

void LedInformation::charging()
{
    setNewStrategy(mBatteryLedStrategy, StaticLED(mBatteryLed, staticLEDState::LED_OFF));
}

void LedInformation::keyLockStatesChanged(const Identifiable& source,
                                          const HIDDefs::KeyLockStates& keyLockStates)
{
    if(keyLockStates.capsLock)
    {
        setNewStrategy(mCapsLockLedStrategy, StaticLED(mCapsLockLed, staticLEDState::LED_ON));
    }
    else
    {
        setNewStrategy(mCapsLockLedStrategy, StaticLED(mCapsLockLed, staticLEDState::LED_OFF));
    }
}

void LedInformation::onUpdateEvent(UpdateStatus status)
{
    AutoLock autoLock(mMutex);

    switch(status)
    {
    case UpdateStatus::START:
        mUpdateInProgress = true;
        stopStrategy(mCapsLockLedStrategy);
        stopStrategy(mBleLedStrategy);
        startStrategy(mUpdateLedStrategy);
        break;
    case UpdateStatus::END:
    case UpdateStatus::ERROR:
        mUpdateInProgress = false;
        stopStrategy(mUpdateLedStrategy);
        startStrategy(mCapsLockLedStrategy);
        startStrategy(mBleLedStrategy);
        break;
    default:
        break;
    }
}

void LedInformation::setLedConfiguration(const KeyboardParameters& params)
{
    /* Strategies are not refreshed while in use */

    mBleBlinkingDelayOnMs = params.getBleBlinkOffDelayMs();
    mBleBlinkingDelayOffMs = params.getBleBlinkOnDelayMs();

    mBatteryBlinkingDelayOnMs = params.getLowBatteryBlinkOnDelayMs();
    mBatteryBlinkingDelayOffMs = params.getLowBatteryBlinkOffDelayMs();
}
