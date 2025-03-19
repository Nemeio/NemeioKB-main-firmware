#pragma once

#include "usbPowerState.h"
#include "IPowerController.hpp"
#include "SystemTypes.hpp"
#include "ListenerSubject/IListenerCollection.hpp"
#include "IPowerManageable.h"
#include "etl/fsm.h"

namespace powerfsm
{
class PowerStateMachine;

static constexpr etl::message_router_id_t POWER_MANAGEMENT_FSM = 1;

namespace EventId
{
enum
{
    TIMEOUT,
    USER_ACTIVITY,
    DEMO_START,
    SYSTEM_ACTIVITY,
    DEMO_STOP,
    POWER_OFF,
    SUSPEND
};
} // namespace EventId

namespace StateId
{
enum
{
    ACTIVE,
    INACTIVE,
    LONG_INACTIVE,
    SLEEP,
    SLEEP_USB_DISCONNECTED,
    OFF,
    DEMO,
    SUSPENDED,
    NUMBER_OF_STATES,
};
} // namespace StateId

class TimeoutEvent : public etl::message<EventId::TIMEOUT>
{
};

class UserActivityEvent : public etl::message<EventId::USER_ACTIVITY>
{
};

class DemoStartEvent : public etl::message<EventId::DEMO_START>
{
};

class SystemActivityEvent : public etl::message<EventId::SYSTEM_ACTIVITY>
{
};

class DemoStopEvent : public etl::message<EventId::DEMO_STOP>
{
};

class PowerOffEvent : public etl::message<EventId::POWER_OFF>
{
};

class SuspendEvent : public etl::message<EventId::SUSPEND>
{
};

class ActiveState : public etl::fsm_state<PowerStateMachine,
                                          ActiveState,
                                          StateId::ACTIVE,
                                          TimeoutEvent,
                                          UserActivityEvent,
                                          DemoStartEvent,
                                          SuspendEvent,
                                          PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    void on_exit_state() final {}
    etl::fsm_state_id_t on_event(const TimeoutEvent& event);
    etl::fsm_state_id_t on_event(const UserActivityEvent& event);
    etl::fsm_state_id_t on_event(const DemoStartEvent& event);
    etl::fsm_state_id_t on_event(const SuspendEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class InactiveState : public etl::fsm_state<PowerStateMachine,
                                            InactiveState,
                                            StateId::INACTIVE,
                                            TimeoutEvent,
                                            UserActivityEvent,
                                            DemoStartEvent,
                                            SuspendEvent,
                                            PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const TimeoutEvent& event);
    etl::fsm_state_id_t on_event(const UserActivityEvent& event);
    etl::fsm_state_id_t on_event(const DemoStartEvent& event);
    etl::fsm_state_id_t on_event(const SuspendEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class LongInactiveState : public etl::fsm_state<PowerStateMachine,
                                                LongInactiveState,
                                                StateId::LONG_INACTIVE,
                                                TimeoutEvent,
                                                UserActivityEvent,
                                                SystemActivityEvent,
                                                DemoStartEvent,
                                                SuspendEvent,
                                                PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    void on_exit_state() final {}
    etl::fsm_state_id_t on_event(const TimeoutEvent& event);
    etl::fsm_state_id_t on_event(const UserActivityEvent& event);
    etl::fsm_state_id_t on_event(const SystemActivityEvent& event);
    etl::fsm_state_id_t on_event(const DemoStartEvent& event);
    etl::fsm_state_id_t on_event(const SuspendEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class SleepState : public etl::fsm_state<PowerStateMachine,
                                         SleepState,
                                         StateId::SLEEP,
                                         UserActivityEvent,
                                         DemoStartEvent,
                                         SuspendEvent,
                                         PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const UserActivityEvent& event);
    etl::fsm_state_id_t on_event(const DemoStartEvent& event);
    etl::fsm_state_id_t on_event(const SuspendEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class SleepUsbDisconnectedState : public etl::fsm_state<PowerStateMachine,
                                                        SleepUsbDisconnectedState,
                                                        StateId::SLEEP_USB_DISCONNECTED,
                                                        TimeoutEvent,
                                                        UserActivityEvent,
                                                        DemoStartEvent,
                                                        SuspendEvent,
                                                        PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const TimeoutEvent& event);
    etl::fsm_state_id_t on_event(const UserActivityEvent& event);
    etl::fsm_state_id_t on_event(const DemoStartEvent& event);
    etl::fsm_state_id_t on_event(const SuspendEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class OffState : public etl::fsm_state<PowerStateMachine, OffState, StateId::OFF>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class DemoState
    : public etl::
          fsm_state<PowerStateMachine, DemoState, StateId::DEMO, DemoStopEvent, SuspendEvent, PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const DemoStopEvent& event);
    etl::fsm_state_id_t on_event(const SuspendEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class SuspendedState : public etl::fsm_state<PowerStateMachine,
                                             SuspendedState,
                                             StateId::SUSPENDED,
                                             UserActivityEvent,
                                             PowerOffEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const UserActivityEvent& event);
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

/*  XXXXTimeout => Keyboard powered
	 *	XXXXUSBDisconnectedTimeout => Keyboard on battery
	 */

struct Timeouts
{
    DelaySec activeTimeout = DelaySec(DelaySec::INFINITE_DELAY);
    DelaySec inactiveTimeout = DelaySec(DelaySec::INFINITE_DELAY);
    DelaySec longInactiveTimeout = DelaySec(0);
    DelaySec activeUSBDisconnectedTimeout = DelaySec(DelaySec::INFINITE_DELAY);
    DelaySec inactiveUSBDisconnectedTimeout = DelaySec(DelaySec::INFINITE_DELAY);
    DelaySec longInactiveUSBDisconnectedTimeout = DelaySec(DelaySec::INFINITE_DELAY);
    DelaySec sleepUSBDisconnectedTimeout = DelaySec(DelaySec::INFINITE_DELAY);

    Timeouts(DelaySec activeTimeout_,
             DelaySec inactiveTimeout_,
             DelaySec longInactiveTimeout_,
             DelaySec activeUSBDisconnectedTimeout_,
             DelaySec inactiveUSBDisconnectedTimeout_,
             DelaySec longInactiveUSBDisconnectedTimeout_,
             DelaySec sleepUSBDisconnectedTimeout_)
        : activeTimeout(activeTimeout_)
        , inactiveTimeout(inactiveTimeout_)
        , longInactiveTimeout(longInactiveTimeout_)
        , activeUSBDisconnectedTimeout(activeUSBDisconnectedTimeout_)
        , inactiveUSBDisconnectedTimeout(inactiveUSBDisconnectedTimeout_)
        , longInactiveUSBDisconnectedTimeout(longInactiveUSBDisconnectedTimeout_)
        , sleepUSBDisconnectedTimeout(sleepUSBDisconnectedTimeout_)
    {
    }

    Timeouts() = default;
};

class PowerStateMachine : public etl::fsm
{
public:
    PowerStateMachine(IPowerController& powerController,
                      IListenerCollection<IPowerManageable*>& listeners);
    virtual ~PowerStateMachine() = default;

    DelaySec getCurrentTimeout() const { return mCurrentTimeout; }

    // -Events-
    void updateTimeouts(const Timeouts& timeouts);
    void updatePowerConnectionState(USB_POWER_CONNECTION_STATE powerConnectionState);
    void timeout();
    void userActivityDetected();
    void systemActivityDetected();
    void startDemo();
    void stopDemo();
    void powerOff();

    // -Actions-
    void doNotifyStateChange();
    void doSetCurrentTimeoutActive();
    void doSetCurrentTimeoutInactive();
    void doSetCurrentTimeoutLongInactive();
    void doSetCurrentTimeoutSleepUSBDisconnected();
    void doSetCurrentTimeoutInfinite() { mCurrentTimeout = DelaySec::INFINITE_DELAY; }
    void doSleep();
    void doShutdown();

    // -Conditions-
    bool isUsbConnected() const;

private:
    IPowerController& mPowerController;
    IListenerCollection<IPowerManageable*>& mListeners;
    USB_POWER_CONNECTION_STATE mPowerConnectionState = NONE;
    Timeouts mTimeouts;
    DelaySec mCurrentTimeout{DelaySec::INFINITE_DELAY};

    ActiveState activeState;
    InactiveState inactiveState;
    LongInactiveState longInactiveState;
    SleepState sleepState;
    SleepUsbDisconnectedState sleepUsbDisconnectedState;
    OffState offState;
    DemoState demoState;
    SuspendedState suspendedState;
    etl::array<etl::ifsm_state*, StateId::NUMBER_OF_STATES> stateList{&activeState,
                                                                      &inactiveState,
                                                                      &longInactiveState,
                                                                      &sleepState,
                                                                      &sleepUsbDisconnectedState,
                                                                      &offState,
                                                                      &demoState,
                                                                      &suspendedState};
    etl::fsm_state_id_t m_previousNotifiedState{StateId::ACTIVE};

    void pluggedInComputer();
    void pluggedInPowerSupplyOnly();
    void pluggedOut();
    void pluggedInComputerSuspended();
};
} // namespace powerfsm