/*
 * BatteryStateMachine.hpp
 *
 *  Created on: 17 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_POWERMANAGER_BATTERYSTATEMACHINE_HPP_
#define NEMEIO_POWERMANAGER_BATTERYSTATEMACHINE_HPP_

#include "IBatteryLevelListener.hpp"
#include "IUsbActivable.h"
#include "IBatteryStateListener.hpp"

class BatteryStateMachine : public IBatteryLevelListener, public IUsbActivable
{
public:
    BatteryStateMachine(IBatteryStateListener& batteryStateListener);
    virtual ~BatteryStateMachine() = default;

    void batteryLevel(uint8_t batteryLevelPercent);
    void usbPluggedInComputer();
    void usbPluggedInComputerSuspended();
    void usbPluggedInPowerSupplyOnly();
    void usbPluggedOut();
    void setLowBatteryLevelThresholdPercent(uint8_t lowBatteryLevelThresholdPercent);

private:
    static constexpr uint8_t DEFAULT_LOW_BATTERY_THRESHOLD_PERCENT = 20;
    enum class BatteryState
    {
        INIT,
        NORMAL,
        LOW,
        CHARGING,
    };

    void pluggedIn();
    void pluggedOut();

    bool isBatteryLow();

    void enterChargingState();
    void enterLowBatteryState();
    void enterNormalBatteryState();

    IBatteryStateListener& mBatteryStateListener;
    BatteryState mState;
    uint8_t mLowBatteryLevelThresholdPercent;
    uint8_t mBatteryLevelPercent;
};

#endif /* NEMEIO_POWERMANAGER_BATTERYSTATEMACHINE_HPP_ */
