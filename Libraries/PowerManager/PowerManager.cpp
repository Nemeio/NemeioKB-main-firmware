/*
 * PowerManager.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: amongatcheucheu
 */

#include "assertTools.h"
#include "PowerManager.h"
#include "Application.hpp"
#include "timing_macros.h"
#include "systemUtils.h"
#include "FreeRTOSUtils.hpp"
#include "TraceLogger.hpp"

#define MODULE "powerman"

QueueHandle_t PowerManager::mActivityQueue = xQueueCreate(10, sizeof(CheckActivityMsg));
QueueHandle_t PowerManager::mPowerManagerQueue = xQueueCreate(10, sizeof(PowerManagerMessage));

PowerManager::PowerManager(ISystem& system,
                           IPowerController& powerController,
                           IListenerCollection<IPowerManageable*>& powerManageables)
    : mSystem(system)
    , mUsbObserverList()
    , mBatteryLevelObserverList()
    , mPowerOffButton(*this, mGPIOController, system)
    , mPowerStateMachine(powerController, powerManageables)
{
    xTaskCreate(vCheckActivityTask,
                "CheckActivityTask",
                CHECK_ACTIVITY_TASK_STACK_SIZE,
                this,
                CHECK_ACTIVITY_TASK_PRIORITY,
                &mCheckActivityTaskHandle);
    xTaskCreate(vPowerManagerTask,
                "PowerManagerTask",
                POWER_MANAGER_TASK_STACK_SIZE,
                this,
                POWER_MANAGER_TASK_PRIORITY,
                &mPowerManagerTaskHandle);
    mSuspendDetectTimer = xTimerCreate("suspendDetectTimer",
                                       delayToSystemTicks(DelayMs(SUSPEND_DETECT_TIMEOUT_MS)),
                                       pdFALSE,
                                       this,
                                       suspendDetectTimerCallback);
}

void PowerManager::pressedKey(const ButtonID& button)
{
}

void PowerManager::unpressedKey(const ButtonID& button)
{
}

void PowerManager::terminateUpdateKeys()
{
    userActivityHappened();
}

void PowerManager::registerUsbObserver(IUsbActivable& observer)
{
    std::vector<IUsbActivable*>::iterator ite = std::find(mUsbObserverList.begin(),
                                                          mUsbObserverList.end(),
                                                          &observer);
    if(mUsbObserverList.end() == ite)
    {
        mUsbObserverList.push_back(&observer);
    }
}

void PowerManager::unregisterUsbObserver(IUsbActivable& observer)
{
    std::vector<IUsbActivable*>::iterator ite = std::find(mUsbObserverList.begin(),
                                                          mUsbObserverList.end(),
                                                          &observer);
    if(mUsbObserverList.end() != ite)
    {
        mUsbObserverList.erase(ite);
    }
}

void PowerManager::registerBatteryLevelObserver(IBatteryLevelListener& observer)
{
    auto ite = std::find(mBatteryLevelObserverList.begin(),
                         mBatteryLevelObserverList.end(),
                         &observer);
    if(mBatteryLevelObserverList.end() == ite)
    {
        mBatteryLevelObserverList.push_back(&observer);
    }
}

void PowerManager::unregisterBatteryLevelObserver(IBatteryLevelListener& observer)
{
    auto ite = std::find(mBatteryLevelObserverList.begin(),
                         mBatteryLevelObserverList.end(),
                         &observer);
    if(mBatteryLevelObserverList.end() != ite)
    {
        mBatteryLevelObserverList.erase(ite);
    }
}

void PowerManager::launch() const
{
    TRACE_FUNC_ENTRY();
    userActivityHappened();
    TRACE_FUNC_EXIT();
}

void PowerManager::updateTimers(uint32_t inactiveTime,
                                uint32_t longInactiveTime,
                                uint32_t sleepTime,
                                uint32_t inactiveTimeUSBDisconnected,
                                uint32_t longInactiveTimeUSBDisconnected,
                                uint32_t sleepTimeUSBDisconnected,
                                uint32_t poweroffTimeUSBDisconnected) const
{
    CheckActivityMsg data;
    data.msgId = CheckActivityMsgId::UPDATE_TIMEOUTS;
    data.updateTimeoutsData.inactiveTime = inactiveTime;
    data.updateTimeoutsData.longInactiveTime = longInactiveTime;
    data.updateTimeoutsData.sleepTime = sleepTime;
    data.updateTimeoutsData.inactiveTimeUSBDisconnected = inactiveTimeUSBDisconnected;
    data.updateTimeoutsData.longInactiveTimeUSBDisconnected = longInactiveTimeUSBDisconnected;
    data.updateTimeoutsData.sleepTimeUSBDisconnected = sleepTimeUSBDisconnected;
    data.updateTimeoutsData.poweroffTimeUSBDisconnected = poweroffTimeUSBDisconnected;

    xQueueSendToBack(mActivityQueue, (void*) &data, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::pushState(USB_POWER_CONNECTION_STATE state)
{
    PowerManagerMessage message = {};
    message.msgId = USB_POWER_STATE;
    message.usbPowerConnectionState = state;

    if(isInISRContext())
    {
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendToBackFromISR(PowerManager::mPowerManagerQueue,
                                reinterpret_cast<void*>(&message),
                                &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        xQueueSendToBack(PowerManager::mPowerManagerQueue,
                         reinterpret_cast<void*>(&message),
                         PowerManager::POWER_MANAGER_QUEUE_DELAY);
    }
}

void PowerManager::notifyUsbSuspend()
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    PowerManagerMessage message = {};
    message.msgId = USB_SUSPEND_EVT;
    xQueueSendToBackFromISR(PowerManager::mPowerManagerQueue,
                            (void*) &message,
                            &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void PowerManager::notifyUsbResume()
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;

    PowerManagerMessage message = {};
    message.msgId = USB_RESUME_EVT;
    xQueueSendToBackFromISR(PowerManager::mPowerManagerQueue,
                            (void*) &message,
                            &xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


void pushUsbState(USB_POWER_CONNECTION_STATE state)
{
    PowerManager::pushState(state);
}

void notifyUsbSuspend()
{
    PowerManager::notifyUsbSuspend();
}

void notifyUsbResume()
{
    PowerManager::notifyUsbResume();
}

void notifyUsbConnect()
{
    PowerManager::notifyUsbConnect();
}

void notifyUsbDisconnect()
{
    PowerManager::notifyUsbDisconnect();
}

void PowerManager::notifyUsbObserver()
{
    if(mUsbPowerCurrentState == PLUG_IN_PC)
    {
        for(std::vector<IUsbActivable*>::iterator ite = mUsbObserverList.begin();
            ite != mUsbObserverList.end();
            ++ite)
        {
            (*ite)->usbPluggedInComputer();
        }
    }
    else if(mUsbPowerCurrentState == PLUG_IN_POWER_SUPPLY_ONLY)
    {
        for(std::vector<IUsbActivable*>::iterator ite = mUsbObserverList.begin();
            ite != mUsbObserverList.end();
            ++ite)
        {
            (*ite)->usbPluggedInPowerSupplyOnly();
        }
    }
    else if(mUsbPowerCurrentState == PLUG_OUT)
    {
        for(std::vector<IUsbActivable*>::iterator ite = mUsbObserverList.begin();
            ite != mUsbObserverList.end();
            ++ite)
        {
            (*ite)->usbPluggedOut();
        }
    }
    else if(mUsbPowerCurrentState == PLUG_IN_PC_SUSPENDED)
    {
        for(std::vector<IUsbActivable*>::iterator ite = mUsbObserverList.begin();
            ite != mUsbObserverList.end();
            ++ite)
        {
            (*ite)->usbPluggedInComputerSuspended();
        }
    }
}

void PowerManager::vCheckActivityTask(void* pvParameters)
{
    ASSERT(pvParameters != nullptr);
    PowerManager& pm = *(reinterpret_cast<PowerManager*>(pvParameters));
    DelaySec delay = DelaySec(DelaySec::INFINITE_DELAY);

    for(;;)
    {
        CheckActivityMsg data;
        if(pdPASS == xQueueReceive(PowerManager::mActivityQueue, &data, delayToSystemTicks(delay)))
        {
            switch(data.msgId)
            {
            case CheckActivityMsgId::POWER_OFF_DETECTED:
                pm.mPowerStateMachine.powerOff();
                break;
            case CheckActivityMsgId::USER_ACTIVITY_DETECTED:
                pm.mPowerStateMachine.userActivityDetected();
                break;
            case CheckActivityMsgId::SYSTEM_ACTIVITY_DETECTED:
                pm.mPowerStateMachine.systemActivityDetected();
                break;
            case CheckActivityMsgId::USB_STATE:
                pm.mPowerStateMachine.updatePowerConnectionState(data.usbPowerConnectionState);
                break;
            case CheckActivityMsgId::DEMO_START:
                pm.mPowerStateMachine.startDemo();
                break;
            case CheckActivityMsgId::DEMO_STOP:
                pm.mPowerStateMachine.stopDemo();
                break;
            case CheckActivityMsgId::UPDATE_TIMEOUTS:
                pm.updatePowerManagerStateMachineTimeouts(data.updateTimeoutsData);
                break;
            default:
                break;
            }
        }
        else
        {
            pm.mPowerStateMachine.timeout();
        }

        delay = pm.mPowerStateMachine.getCurrentTimeout();
    }
}

void PowerManager::userActivityHappened() const
{
    CheckActivityMsg data = {.msgId = CheckActivityMsgId::USER_ACTIVITY_DETECTED};
    xQueueSendToBack(mActivityQueue, (void*) &data, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::systemActivityHappened() const
{
    CheckActivityMsg data = {.msgId = CheckActivityMsgId::SYSTEM_ACTIVITY_DETECTED};
    xQueueSendToBack(mActivityQueue, (void*) &data, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::powerOff()
{
    CheckActivityMsg data = {.msgId = CheckActivityMsgId::POWER_OFF_DETECTED};

    if(isInISRContext())
    {
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendToBackFromISR(PowerManager::mActivityQueue, &data, &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        xQueueSendToBack(PowerManager::mActivityQueue,
                         &data,
                         PowerManager::POWER_MANAGER_QUEUE_DELAY);
    }
}

void PowerManager::batteryLevel(uint8_t batteryLevelPercentage)
{
    PowerManagerMessage message = {};
    message.msgId = BATTERY_LEVEL;
    message.batteryLevel = batteryLevelPercentage;

    xQueueSendToBack(mPowerManagerQueue, (void*) &message, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::notifyBatteryLevelObserver(uint8_t batteryLevel)
{
    for(auto ite = mBatteryLevelObserverList.begin(); ite != mBatteryLevelObserverList.end(); ++ite)
    {
        (*ite)->batteryLevel(batteryLevel);
    }
}

void PowerManager::updateUSBPowerState(USB_POWER_CONNECTION_STATE usbPowerState)
{
    if(mUsbPowerCurrentState != usbPowerState)
    {
        mUsbPowerCurrentState = usbPowerState;

        xTimerStop(mSuspendDetectTimer, 0);

        updateUSBPowerStateToCheckActivityTask(mUsbPowerCurrentState);

        if(usbPowerState == PLUG_OUT)
        {
            LOG_INFO(MODULE, "USB plugged out");
        }
        else if(usbPowerState == PLUG_IN_POWER_SUPPLY_ONLY)
        {
            LOG_INFO(MODULE, "USB plugged into power supply");
        }
        else if(usbPowerState == PLUG_IN_PC)
        {
            LOG_INFO(MODULE, "USB plugged into PC");
        }
        else if(usbPowerState == PLUG_IN_PC_SUSPENDED)
        {
            LOG_INFO(MODULE, "USB host suspended");
        }
        notifyUsbObserver();
    }
}

void PowerManager::usbSuspend()
{
    mUSBActivityState = USB_SUSPENDED;

    if(mUsbPowerCurrentState == PLUG_IN_PC)
    {
        // VBUS may take some time to go to low state if the USB is plugged out from PC
        // Wait some time to check that the USB is really still plugged in
        xTimerStart(mSuspendDetectTimer, 0);
    }
}

void PowerManager::usbResume()
{
    mUSBActivityState = USB_ACTIVE;
    xTimerStop(mSuspendDetectTimer, 0);
    if(mUsbPowerCurrentState == PLUG_IN_PC_SUSPENDED)
    {
        updateUSBPowerState(PLUG_IN_PC);
    }
}

void PowerManager::suspendDetectTimerCallback(TimerHandle_t xTimer)
{
    PowerManager* pm = (PowerManager*) pvTimerGetTimerID(xTimer);
    if(pm != NULL && pm->mUSBActivityState == USB_SUSPENDED
       && pm->mUsbPowerCurrentState == PLUG_IN_PC)
    {
        PowerManagerMessage message = {};
        message.msgId = USB_POWER_STATE;
        message.usbPowerConnectionState = PLUG_IN_PC_SUSPENDED;
        xQueueSendToBack(mPowerManagerQueue, (void*) &message, (TickType_t) 0);
    }
}

void PowerManager::demoStart()
{
    CheckActivityMsg data = {.msgId = CheckActivityMsgId::DEMO_START};
    xQueueSendToBack(mActivityQueue, (void*) &data, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::demoStop()
{
    CheckActivityMsg data = {.msgId = CheckActivityMsgId::DEMO_STOP};
    xQueueSendToBack(mActivityQueue, (void*) &data, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::initUsbPowerState()
{
    pushState(PLUG_OUT);
}

void PowerManager::updateUSBPowerStateToCheckActivityTask(
    USB_POWER_CONNECTION_STATE usbPowerState) const
{
    CheckActivityMsg data;
    data.msgId = CheckActivityMsgId::USB_STATE;
    data.usbPowerConnectionState = usbPowerState;
    xQueueSendToBack(mActivityQueue, (void*) &data, POWER_MANAGER_QUEUE_DELAY);
}

void PowerManager::updatePowerManagerStateMachineTimeouts(const UpdateTimeoutsData& timeoutsData)
{
    /* Calculate delta between two modes */
    int32_t longInactiveTimeout = timeoutsData.longInactiveTime - timeoutsData.inactiveTime;
    int32_t sleepTimeout = timeoutsData.sleepTime - timeoutsData.longInactiveTime;

    int32_t USBDisconnectedLongInactiveTimeout = timeoutsData.longInactiveTimeUSBDisconnected
                                                 - timeoutsData.inactiveTimeUSBDisconnected;
    int32_t USBDisconnectedSleepTimeout = timeoutsData.sleepTimeUSBDisconnected
                                          - timeoutsData.longInactiveTimeUSBDisconnected;

    int32_t powerOffUSBDisconnectedTimeout = timeoutsData.poweroffTimeUSBDisconnected
                                             - timeoutsData.sleepTimeUSBDisconnected;

    /* Check if values are valid */
    if(sleepTimeout < 0 || longInactiveTimeout < 0 || USBDisconnectedSleepTimeout < 0
       || USBDisconnectedLongInactiveTimeout < 0 || powerOffUSBDisconnectedTimeout < 0)
    {
        mPowerStateMachine.systemActivityDetected();
    }

    /* Update timeouts */
    powerfsm::Timeouts timeouts(DelaySec(static_cast<int32_t>(timeoutsData.inactiveTime)),
                                DelaySec(static_cast<int32_t>(longInactiveTimeout)),
                                DelaySec(static_cast<int32_t>(sleepTimeout)),
                                DelaySec(
                                    static_cast<int32_t>(timeoutsData.inactiveTimeUSBDisconnected)),
                                DelaySec(static_cast<int32_t>(USBDisconnectedLongInactiveTimeout)),
                                DelaySec(static_cast<int32_t>(USBDisconnectedSleepTimeout)),
                                DelaySec(static_cast<int32_t>(powerOffUSBDisconnectedTimeout)));

    mPowerStateMachine.updateTimeouts(timeouts);
}

void PowerManager::notifyUsbConnect()
{
    pushUsbState(PLUG_IN_POWER_SUPPLY_ONLY);
}

void PowerManager::notifyUsbDisconnect()
{
    pushUsbState(PLUG_OUT);
}

void PowerManager::vPowerManagerTask(void* pvParameters)
{
    PowerManager* pm = (PowerManager*) pvParameters;
    TickType_t delay = portMAX_DELAY;
    PowerManagerMessage data;
    for(;;)
    {
        if(pdPASS == xQueueReceive(PowerManager::mPowerManagerQueue, &data, delay))
        {
            switch(data.msgId)
            {
            case BATTERY_LEVEL:
                pm->notifyBatteryLevelObserver(data.batteryLevel);
                break;
            case USB_POWER_STATE:
                pm->updateUSBPowerState(data.usbPowerConnectionState);
                break;
            case USB_SUSPEND_EVT:
                pm->usbSuspend();
                break;
            case USB_RESUME_EVT:
                pm->usbResume();
                break;
            default:
                break;
            }
        }
    }
}

void PowerManager::init()
{
    mPowerOffButton.init();
    initUsbPowerState();
}
