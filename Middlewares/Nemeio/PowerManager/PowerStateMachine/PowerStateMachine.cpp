#include "PowerStateMachine.hpp"
#include "utils.hpp"

using namespace powerfsm;

etl::fsm_state_id_t ActiveState::on_event(const TimeoutEvent&)
{
    return StateId::INACTIVE;
}

etl::fsm_state_id_t ActiveState::on_event(const UserActivityEvent&)
{
    // fsm does not currently manage self-transitions
    on_exit_state();
    on_enter_state();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t ActiveState::on_event(const DemoStartEvent&)
{
    return StateId::DEMO;
}

etl::fsm_state_id_t ActiveState::on_event(const SuspendEvent&)
{
    return StateId::SUSPENDED;
}


etl::fsm_state_id_t ActiveState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t ActiveState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutActive();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t InactiveState::on_event(const TimeoutEvent&)
{
    return StateId::LONG_INACTIVE;
}

etl::fsm_state_id_t InactiveState::on_event(const UserActivityEvent&)
{
    return StateId::ACTIVE;
}

etl::fsm_state_id_t InactiveState::on_event(const DemoStartEvent&)
{
    return StateId::DEMO;
}

etl::fsm_state_id_t InactiveState::on_event(const SuspendEvent&)
{
    return StateId::SUSPENDED;
}

etl::fsm_state_id_t InactiveState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t InactiveState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutInactive();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t LongInactiveState::on_event(const TimeoutEvent&)
{
    return get_fsm_context().isUsbConnected() ? StateId::SLEEP : StateId::SLEEP_USB_DISCONNECTED;
}

etl::fsm_state_id_t LongInactiveState::on_event(const UserActivityEvent&)
{
    return StateId::ACTIVE;
}

etl::fsm_state_id_t LongInactiveState::on_event(const SystemActivityEvent&)
{
    // fsm does not currently manage self-transitions
    on_exit_state();
    on_enter_state();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t LongInactiveState::on_event(const DemoStartEvent&)
{
    return StateId::DEMO;
}

etl::fsm_state_id_t LongInactiveState::on_event(const SuspendEvent&)
{
    return StateId::SUSPENDED;
}

etl::fsm_state_id_t LongInactiveState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t LongInactiveState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutLongInactive();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t SleepState::on_event(const UserActivityEvent&)
{
    return StateId::ACTIVE;
}

etl::fsm_state_id_t SleepState::on_event(const DemoStartEvent&)
{
    return StateId::DEMO;
}

etl::fsm_state_id_t SleepState::on_event(const SuspendEvent&)
{
    return StateId::SUSPENDED;
}

etl::fsm_state_id_t SleepState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t SleepState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutInfinite();
    get_fsm_context().doSleep();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t SleepUsbDisconnectedState::on_event(const TimeoutEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t SleepUsbDisconnectedState::on_event(const UserActivityEvent&)
{
    return StateId::ACTIVE;
}

etl::fsm_state_id_t SleepUsbDisconnectedState::on_event(const DemoStartEvent&)
{
    return StateId::DEMO;
}

etl::fsm_state_id_t SleepUsbDisconnectedState::on_event(const SuspendEvent&)
{
    return StateId::SUSPENDED;
}

etl::fsm_state_id_t SleepUsbDisconnectedState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t SleepUsbDisconnectedState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutSleepUSBDisconnected();
    get_fsm_context().doSleep();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t OffState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutInfinite();
    get_fsm_context().doShutdown();
    return etl::ifsm_state::No_State_Change;
}


etl::fsm_state_id_t DemoState::on_event(const DemoStopEvent&)
{
    return StateId::ACTIVE;
}

etl::fsm_state_id_t DemoState::on_event(const SuspendEvent&)
{
    return StateId::SUSPENDED;
}

etl::fsm_state_id_t DemoState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t DemoState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutInfinite();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t SuspendedState::on_event(const UserActivityEvent&)
{
    return StateId::ACTIVE;
}

etl::fsm_state_id_t SuspendedState::on_event(const PowerOffEvent&)
{
    return StateId::OFF;
}

etl::fsm_state_id_t SuspendedState::on_enter_state()
{
    get_fsm_context().doNotifyStateChange();
    get_fsm_context().doSetCurrentTimeoutInfinite();
    return etl::ifsm_state::No_State_Change;
}

PowerStateMachine::PowerStateMachine(IPowerController& powerController,
                                     IListenerCollection<IPowerManageable*>& listeners)
    : etl::fsm(POWER_MANAGEMENT_FSM)
    , mPowerController(powerController)
    , mListeners(listeners)
{
    set_states(stateList.data(), stateList.size());

    start(false);
}

void PowerStateMachine::updateTimeouts(const Timeouts& timeouts)
{
    mTimeouts = timeouts;
    userActivityDetected(); /* Forcing new state to update timeout */
}

void PowerStateMachine::doSetCurrentTimeoutActive()
{
    mCurrentTimeout = isUsbConnected() ? mTimeouts.activeTimeout
                                       : mTimeouts.activeUSBDisconnectedTimeout;
}

void PowerStateMachine::doSetCurrentTimeoutInactive()
{
    mCurrentTimeout = isUsbConnected() ? mTimeouts.inactiveTimeout
                                       : mTimeouts.inactiveUSBDisconnectedTimeout;
}

void PowerStateMachine::doSetCurrentTimeoutLongInactive()
{
    mCurrentTimeout = isUsbConnected() ? mTimeouts.longInactiveTimeout
                                       : mTimeouts.longInactiveUSBDisconnectedTimeout;
}

void PowerStateMachine::doSetCurrentTimeoutSleepUSBDisconnected()
{
    mCurrentTimeout = isUsbConnected() ? DelaySec(DelaySec::INFINITE_DELAY)
                                       : mTimeouts.sleepUSBDisconnectedTimeout;
}

bool PowerStateMachine::isUsbConnected() const
{
    return (PLUG_IN_PC == mPowerConnectionState) || (PLUG_IN_PC_SUSPENDED == mPowerConnectionState)
           || (PLUG_IN_POWER_SUPPLY_ONLY == mPowerConnectionState);
}

void PowerStateMachine::updatePowerConnectionState(USB_POWER_CONNECTION_STATE powerConnectionState)
{
    mPowerConnectionState = powerConnectionState;

    switch(mPowerConnectionState)
    {
    case NONE:
        break;
    case PLUG_IN_PC:
        pluggedInComputer();
        break;
    case PLUG_IN_PC_SUSPENDED:
        pluggedInComputerSuspended();
        break;
    case PLUG_IN_POWER_SUPPLY_ONLY:
        pluggedInPowerSupplyOnly();
        break;
    case PLUG_OUT:
        pluggedOut();
        break;
    default:
        break;
    }
}

void PowerStateMachine::doShutdown()
{
    mPowerController.shutdown();
}

void PowerStateMachine::doSleep()
{
    mPowerController.sleep();
}

void PowerStateMachine::timeout()
{
    TimeoutEvent event;
    receive(event);
}

void PowerStateMachine::userActivityDetected()
{
    UserActivityEvent event;
    receive(event);
}

void PowerStateMachine::systemActivityDetected()
{
    SystemActivityEvent event;
    receive(event);
}

void PowerStateMachine::startDemo()
{
    DemoStartEvent event;
    receive(event);
}

void PowerStateMachine::stopDemo()
{
    DemoStopEvent event;
    receive(event);
}

void PowerStateMachine::powerOff()
{
    PowerOffEvent event;
    receive(event);
}

void PowerStateMachine::pluggedInComputer()
{
    userActivityDetected();
}

void PowerStateMachine::pluggedInPowerSupplyOnly()
{
    userActivityDetected();
}

void PowerStateMachine::pluggedOut()
{
    userActivityDetected();
}

void PowerStateMachine::pluggedInComputerSuspended()
{
    SuspendEvent event;
    receive(event);
}

void PowerStateMachine::doNotifyStateChange()
{
    etl::fsm_state_id_t state = get_state_id();

    if(state != m_previousNotifiedState)
    {
        mListeners.notify(
            [state](IPowerManageable* listener)
            {
                if(listener == nullptr)
                {
                    return;
                }

                switch(state)
                {
                case StateId::ACTIVE:
                    listener->active();
                    break;
                case StateId::INACTIVE:
                    listener->inactive();
                    break;
                case StateId::SLEEP:
                case StateId::SLEEP_USB_DISCONNECTED:
                    listener->sleep();
                    break;
                case StateId::LONG_INACTIVE:
                    listener->longInactive();
                    break;
                case StateId::OFF:
                    listener->powerOff();
                    break;
                case StateId::SUSPENDED:
                    listener->suspend();
                    break;
                default:
                    break;
                }
            });

        m_previousNotifiedState = state;
    }
}