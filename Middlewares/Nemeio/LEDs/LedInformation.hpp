/*
 * LedInformation.hpp
 *
 *  Created on: 4 mars 2021
 *      Author: thomas
 */

#ifndef LEDS_LEDINFORMATION_HPP_
#define LEDS_LEDINFORMATION_HPP_

#include "etl/variant.h"
#include "ILED.hpp"
#include "IBatteryStateListener.hpp"
#include "BleEventObserver.hpp"
#include "IKeyLockStatesListener.hpp"
#include "KeyboardParameters.hpp"
#include "ITimer.hpp"

#include "Strategies/StaticLED.hpp"
#include "Strategies/BlinkingLED.hpp"
#include "Strategies/AlternateLED.hpp"
#include "Strategies/SynchronizedBlinkingLED.hpp"
#include "IUpdateListener.hpp"
#include "Mutex.hpp"

class LedInformation : public IBatteryStateListener,
                       public BleEventObserver,
                       public IKeyLockStatesListener,
                       public IUpdateListener
{
public:
    LedInformation(ILED& capsLock,
                   ILED& battery,
                   ILED& ble,
                   ITimer& batteryTimer,
                   ITimer& bleTimer,
                   ITimer& updateTimer);
    virtual ~LedInformation() = default;

    void configure(KeyboardParameters& params);

    /* IBatteryStateListener */
    void lowBatteryState() override;
    void normalBatteryState() override;
    void charging() override;

    /* BleEventObserver */
    void notification(BleEvent event) final;

    /* IKeyLockStateListener */
    void keyLockStatesChanged(const Identifiable& source,
                              const HIDDefs::KeyLockStates& keyLockStates) override;

    /* Update Listener */
    void onUpdateEvent(UpdateStatus status) final;

    void setLedConfiguration(const KeyboardParameters& params);

private:
    /* Blinking timing */
    static constexpr int32_t DEFAULT_BLE_BLINKING_DELAY_ON_MS = 200;
    static constexpr int32_t DEFAULT_BLE_BLINKING_DELAY_OFF_MS = 1800;

    static constexpr int32_t DEFAULT_BATTERY_BLINKING_DELAY_ON_MS = 100;
    static constexpr int32_t DEFAULT_BATTERY_BLINKING_DELAY_OFF_MS = 900;

    static constexpr int32_t DEFAULT_UPDATE_BLINKING_DELAY_MS = 900;

    int32_t mBatteryBlinkingDelayOnMs = DEFAULT_BATTERY_BLINKING_DELAY_ON_MS;
    int32_t mBatteryBlinkingDelayOffMs = DEFAULT_BATTERY_BLINKING_DELAY_ON_MS;
    int32_t mBleBlinkingDelayOnMs = DEFAULT_BLE_BLINKING_DELAY_ON_MS;
    int32_t mBleBlinkingDelayOffMs = DEFAULT_BLE_BLINKING_DELAY_ON_MS;
    int32_t mUpdateBlinkingDelayMs = DEFAULT_UPDATE_BLINKING_DELAY_MS;

    Mutex mMutex;
    bool mUpdateInProgress = false;

    /* Leds */
    ILED& mCapsLockLed;
    ILED& mBatteryLed;
    ILED& mBleLed;

    /* Timers */
    ITimer& mBatteryLedTimer;
    ITimer& mBleLedTimer;
    ITimer& mUpdateLedTimer;

    using ledStrategy = etl::variant<StaticLED, BlinkingLED, AlternateLED, SynchronizedBlinkingLED>;

    /* Strategies */
    ledStrategy mUpdateLedStrategy;
    ledStrategy mCapsLockLedStrategy;
    ledStrategy mBatteryLedStrategy;
    ledStrategy mBleLedStrategy;

    void startStrategy(ledStrategy& strategy) const;
    void stopStrategy(ledStrategy& strategy) const;

    void setNewStrategy(ledStrategy& strategy, ledStrategy&& newStrategy);
};

#endif /* LEDS_LEDINFORMATION_HPP_ */
