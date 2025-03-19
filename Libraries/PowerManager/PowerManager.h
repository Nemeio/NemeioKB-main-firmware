/*
 * PowerManager.h
 *
 *  Created on: Feb 28, 2018
 *      Author: amongatcheucheu
 */

#ifndef POWERMANAGER_POWERMANAGER_H_
#define POWERMANAGER_POWERMANAGER_H_

#include <IDemoListener.hpp>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

#include <vector>
#include <algorithm>
#include "IPowerManageable.h"
#include "STM32GPIOController.hpp"

#include "usbPowerState.h"

#include "IUsbActivable.h"
#include "IBatteryLevelListener.hpp"
#include "iNemeioKeypadPressedListener.h"
#include "ISystem.hpp"
#include "IPowerManager.hpp"
#include "PowerOffButton.hpp"
#include "PowerStateMachine.hpp"

class PowerManager : public IPowerManager,
                     public iNemeioKeypadPressedListener,
                     public IBatteryLevelListener,
                     public IDemoListener
{
public:
    PowerManager(ISystem& system,
                 IPowerController& powerController,
                 IListenerCollection<IPowerManageable*>& powerManageables);
    virtual ~PowerManager() = default;

    void pressedKey(const ButtonID& button);
    void longPressedKey(const ButtonID& button);
    void unpressedKey(const ButtonID& button);
    void terminateUpdateKeys();

    void registerUsbObserver(IUsbActivable& observer);
    void unregisterUsbObserver(IUsbActivable& observer);

    void registerBatteryLevelObserver(IBatteryLevelListener& observer);
    void unregisterBatteryLevelObserver(IBatteryLevelListener& observer);

    void init();
    void launch() const;

    void updateTimers(uint32_t inactiveTime,
                      uint32_t longInactiveTime,
                      uint32_t sleepTime,
                      uint32_t inactiveTimeUSBDisconnected,
                      uint32_t longInactiveTimeUSBDisconnected,
                      uint32_t sleepTimeUSBDisconnected,
                      uint32_t poweroffTimeUSBDisconnected) const;

    void batteryLevel(uint8_t batteryLevelPercentage);

    static const uint32_t INACTIVE_TIME = 12;   //12 sec
    static const uint32_t SLEEP_TIME = 60;      //1 min
    static const uint32_t POWER_OFF_TIME = 300; //5 min
    static void pushState(USB_POWER_CONNECTION_STATE state);
    static void notifyUsbSuspend();
    static void notifyUsbResume();
    static void notifyUsbConnect();
    static void notifyUsbDisconnect();

    void powerOff();

    void demoStart();
    void demoStop();

    void userActivityHappened() const final;
    void systemActivityHappened() const final;

private:
    static constexpr UBaseType_t CHECK_ACTIVITY_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE CHECK_ACTIVITY_TASK_STACK_SIZE = 512;
    static constexpr UBaseType_t POWER_MANAGER_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE POWER_MANAGER_TASK_STACK_SIZE = 512;
    static constexpr TickType_t POWER_MANAGER_QUEUE_DELAY = 10;

    static constexpr int32_t SUSPEND_DETECT_TIMEOUT_MS = 1000;
    static constexpr uint8_t LOW_BATTERY_PERCENTAGE_THRESHOLD_PERCENT = 20;

    enum BatteryLevelState
    {
        BATTERY_LEVEL_LOW,
        BATTERY_LEVEL_NORMAL
    };

    enum PowerManagerMessageID
    {
        BATTERY_LEVEL,
        USB_POWER_STATE,
        USB_SUSPEND_EVT,
        USB_RESUME_EVT,
        USB_PC_SUSPEND_DETECTED,
    };

    struct PowerManagerMessage
    {
        PowerManagerMessageID msgId;
        union
        {
            uint8_t batteryLevel;
            USB_POWER_CONNECTION_STATE usbPowerConnectionState;
        };
    };

    enum USBActivityState
    {
        USB_ACTIVE,
        USB_SUSPENDED,
    };

    enum class CheckActivityMsgId
    {
        POWER_OFF_DETECTED,
        USER_ACTIVITY_DETECTED,
        SYSTEM_ACTIVITY_DETECTED,
        USB_STATE,
        DEMO_START,
        DEMO_STOP,
        UPDATE_TIMEOUTS,
        MAX
    };

    struct UpdateTimeoutsData
    {
        uint32_t inactiveTime;
        uint32_t longInactiveTime;
        uint32_t sleepTime;
        uint32_t inactiveTimeUSBDisconnected;
        uint32_t longInactiveTimeUSBDisconnected;
        uint32_t sleepTimeUSBDisconnected;
        uint32_t poweroffTimeUSBDisconnected;
    };

    struct CheckActivityMsg
    {
        CheckActivityMsgId msgId;
        union
        {
            USB_POWER_CONNECTION_STATE usbPowerConnectionState;
            UpdateTimeoutsData updateTimeoutsData;
        };
    };

    ISystem& mSystem;

    STM32GPIOController mGPIOController;

    USB_POWER_CONNECTION_STATE mUsbPowerCurrentState = NONE;
    USBActivityState mUSBActivityState = USB_SUSPENDED;

    std::vector<IUsbActivable*> mUsbObserverList;
    std::vector<IBatteryLevelListener*> mBatteryLevelObserverList;

    PowerOffButton mPowerOffButton;

    TaskHandle_t mCheckActivityTaskHandle;
    TaskHandle_t mPowerManagerTaskHandle;

    powerfsm::PowerStateMachine mPowerStateMachine;

    void updateUSBPowerStateToCheckActivityTask(USB_POWER_CONNECTION_STATE usbPowerState) const;
    void notifyUsbObserver();
    void notifyBatteryLevelObserver(uint8_t batteryLevel);

    void updateUSBPowerState(USB_POWER_CONNECTION_STATE usbPowerState);
    void usbSuspend();
    void usbResume();

    void initUsbPowerState();

    void updatePowerManagerStateMachineTimeouts(const UpdateTimeoutsData& timeoutsData);

    static QueueHandle_t mActivityQueue;
    static QueueHandle_t mPowerManagerQueue;

    TimerHandle_t mSuspendDetectTimer;

    static void suspendDetectTimerCallback(TimerHandle_t xTimer);

    static void vCheckActivityTask(void* pvParameters);
    static void vPowerManagerTask(void* pvParameters);
};

#endif /* POWERMANAGER_POWERMANAGER_H_ */
