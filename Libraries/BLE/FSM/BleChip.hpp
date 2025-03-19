#pragma once

#include "IBleChip.hpp"
#include "IBleCommunication.hpp"
#include "BleEventCmdObserver.hpp"
#include "BleStartedCmdObserver.hpp"
#include "BleEnteredDfuCmdObserver.hpp"
#include "Hfsm.hpp"
#include <etl/vector.h>
#include <etl/optional.h>
#include "BleNrfStartedCmd.hpp"
#include "BleEventCmd.hpp"
#include "BleEnteredDfuCmd.hpp"
#include "BleHidReportDescriptorCmd.hpp"
#include "BleSetDeviceNameCmd.hpp"
#include "BleSetMacAddressCmd.hpp"
#include "BleSetDevicePropertiesCmd.hpp"
#include "cpp_helper.hpp"
#include "Module.hpp"

class BleChip;

namespace blehfsm
{
static constexpr etl::message_router_id_t BLE_HFSM = 0;

// enum class cannot be used here
namespace EventId
{
enum
{
    INIT,
    POWER_OFF,
    CONFIGURE,
    ENABLE_BLE,
    DISABLE_BLE,
    ADVERTISING_STARTED,
    ADVERTISING_STOPPED,
    CONNECTED,
    DISCONNECTED,
    STARTED,
    ENTERED_DFU,
    SEND_BLE_COMMAND,
    SEND_APPLICATION_COMMAND,
    SEND_APPLICATION_REQUEST,
    SEND_DFU_COMMAND
};
} // namespace EventId

namespace StateId
{
enum
{
    OFF,
    ON,
    STARTED,
    DFU,
    READY,
    // READY child states
    CONFIGURED,
    ENABLED,
    // ENABLED child states
    STARTING_ADVERTISING,
    ADVERTISING,
    CONNECTED,
    NUMBER_OF_STATES,
};
} // namespace StateId

class InitEvent : public etl::message<EventId::INIT>
{
};

class PowerOffEvent : public etl::message<EventId::POWER_OFF>
{
};

class ConfigureEvent : public etl::message<EventId::CONFIGURE>
{
};

class EnableBleEvent : public etl::message<EventId::ENABLE_BLE>
{
};

class DisableBleEvent : public etl::message<EventId::DISABLE_BLE>
{
};

class AdvertisingStartedEvent : public etl::message<EventId::ADVERTISING_STARTED>
{
};

class AdvertisingStoppedEvent : public etl::message<EventId::ADVERTISING_STOPPED>
{
};

class ConnectedEvent : public etl::message<EventId::CONNECTED>
{
};

class DisconnectedEvent : public etl::message<EventId::DISCONNECTED>
{
};

class StartedEvent : public etl::message<EventId::STARTED>
{
};

class EnteredDfuEvent : public etl::message<EventId::ENTERED_DFU>
{
};

template<typename BleCmdType, etl::message_id_t Id>
class SendBleCommandEventBase : public etl::message<Id>
{
public:
    SendBleCommandEventBase(BleCmdType& _cmd, BLEDefs::ErrCode& _ret)
        : cmd(_cmd)
        , ret(_ret)
    {
    }

    BleCmdType& cmd;
    BLEDefs::ErrCode& ret;
};

class SendBleCommandEvent : public SendBleCommandEventBase<BleTxCmd, EventId::SEND_BLE_COMMAND>
{
    using SendBleCommandEventBase::SendBleCommandEventBase;
};

class SendApplicationCommandEvent
    : public SendBleCommandEventBase<BleTxCmd, EventId::SEND_APPLICATION_COMMAND>
{
    using SendBleCommandEventBase::SendBleCommandEventBase;
};

class SendApplicationRequestEvent
    : public SendBleCommandEventBase<BleTxRequest, EventId::SEND_APPLICATION_REQUEST>
{
    using SendBleCommandEventBase::SendBleCommandEventBase;
};

class SendDfuCommandEvent : public SendBleCommandEventBase<BleTxCmd, EventId::SEND_DFU_COMMAND>
{
    using SendBleCommandEventBase::SendBleCommandEventBase;
};

// The states.

class Off : public etl::fsm_state<BleChip, Off, StateId::OFF, InitEvent>
{
public:
    etl::fsm_state_id_t on_event(const InitEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class On
    : public etl::fsm_state<BleChip, On, StateId::ON, PowerOffEvent, StartedEvent, EnteredDfuEvent>
{
public:
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event(const StartedEvent& event);
    etl::fsm_state_id_t on_event(const EnteredDfuEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Started : public etl::fsm_state<BleChip,
                                      Started,
                                      StateId::STARTED,
                                      PowerOffEvent,
                                      StartedEvent,
                                      ConfigureEvent,
                                      EnteredDfuEvent,
                                      SendApplicationCommandEvent,
                                      SendApplicationRequestEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event(const StartedEvent& event);
    etl::fsm_state_id_t on_event(const EnteredDfuEvent& event);
    etl::fsm_state_id_t on_event(const ConfigureEvent& event);
    etl::fsm_state_id_t on_event(const SendApplicationCommandEvent& event);
    etl::fsm_state_id_t on_event(const SendApplicationRequestEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Dfu : public etl::fsm_state<BleChip, Dfu, StateId::DFU, PowerOffEvent, SendDfuCommandEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event(const SendDfuCommandEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Ready : public etl::fsm_state<BleChip,
                                    Ready,
                                    StateId::READY,
                                    PowerOffEvent,
                                    EnteredDfuEvent,
                                    SendApplicationCommandEvent,
                                    SendApplicationRequestEvent,
                                    SendBleCommandEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const PowerOffEvent& event);
    etl::fsm_state_id_t on_event(const EnteredDfuEvent& event);
    etl::fsm_state_id_t on_event(const SendApplicationCommandEvent& event);
    etl::fsm_state_id_t on_event(const SendApplicationRequestEvent& event);
    etl::fsm_state_id_t on_event(const SendBleCommandEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Configured
    : public etl::fsm_state<BleChip, Configured, StateId::CONFIGURED, StartedEvent, EnableBleEvent>
{
public:
    etl::fsm_state_id_t on_event(const StartedEvent& event);
    etl::fsm_state_id_t on_event(const EnableBleEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Enabled
    : public etl::
          fsm_state<BleChip, Enabled, StateId::ENABLED, StartedEvent, DisableBleEvent, EnteredDfuEvent>
{
public:
    void on_exit_state() final;
    etl::fsm_state_id_t on_event(const StartedEvent& event);
    etl::fsm_state_id_t on_event(const EnteredDfuEvent& event);
    etl::fsm_state_id_t on_event(const DisableBleEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class StartingAdvertising : public etl::fsm_state<BleChip,
                                                  StartingAdvertising,
                                                  StateId::STARTING_ADVERTISING,
                                                  AdvertisingStartedEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    etl::fsm_state_id_t on_event(const AdvertisingStartedEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Advertising
    : public etl::
          fsm_state<BleChip, Advertising, StateId::ADVERTISING, ConnectedEvent, AdvertisingStoppedEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    void on_exit_state() final;
    etl::fsm_state_id_t on_event(const ConnectedEvent& event);
    etl::fsm_state_id_t on_event(const AdvertisingStoppedEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};

class Connected : public etl::fsm_state<BleChip, Connected, StateId::CONNECTED, DisconnectedEvent>
{
public:
    etl::fsm_state_id_t on_enter_state() final;
    void on_exit_state() final;
    etl::fsm_state_id_t on_event(const DisconnectedEvent& event);
    etl::fsm_state_id_t on_event_unknown(const etl::imessage&)
    {
        return etl::ifsm_state::No_State_Change;
    }
};
} // namespace blehfsm

class BleChip : public IBleChip,
                public Hfsm,
                public BleEventCmdObserver,
                public BleStartedCmdObserver,
                public BleEnteredDfuCmdObserver,
                public IPowerManageable,
                public Module<Identification::ModuleId::BLE>
{
public:
    explicit BleChip(IBleCommunication& bleCommunication);
    virtual ~BleChip();
    COPYABLE_MOVABLE(BleChip, delete);

    void registerBleCommandListener(AbstractBleCommandListener& commandListener) final;
    void unregisterBleCommandListener(AbstractBleCommandListener& commandListener) final;

    void setHidReportMap(etl::array_view<const uint16_t> reportLengths,
                         etl::array_view<const uint8_t> reportMap) final;
    void setDeviceName(etl::string_view name) final;
    void setDeviceMacAddress(const MacAddress& mac) final;
    void setDeviceProperties(const BLEDefs::bleDeviceProperties& deviceProperties) final;

    void enterDfu() final;
    void forceDfu() final;

    // -Events-

    BLEDefs::ErrCode sendCommand(BleReadyForBleTxCommandRef cmd) final;
    BLEDefs::ErrCode sendCommand(BleApplicationTxCommandRef cmd) final;
    BLEDefs::ErrCode sendCommand(BleDfuModeTxCommandRef cmd) final;
    BLEDefs::ErrCode sendRequest(BleApplicationTxRequestRef cmd) final;

    // IBleChip methods
    void init() final;
    void configure() final;
    void enableBle() final;
    void disableBle() final;

    // BleStartedCmdObserver methods
    void notification(const BleStartedEvent& event) final;

    // BleEventCmdObserver methods
    void notification(BleEventCmdEvent event) final;

    // BleEnteredDfuCmdObserver methods
    void notification(BleEnteredDfuEvent event) final;

    // IPowerManageable methods
    void active() final
    { /* Nothing to do */
    }
    void inactive() final
    { /* Nothing to do */
    }
    void longInactive() final
    { /* Nothing to do */
    }
    void sleep() final
    { /* Nothing to do */
    }
    void powerOff() final;
    void suspend() final
    { /* Nothing to do */
    }

    // -Actions-

    void doInit();
    void doConfigure();
    void doStartAdvertising();
    void doStopAdvertising();
    void doNotifyStarted();
    void doNotifyReady();
    void doNotifyEnteredDfu();
    void doNotifyAdvertisingStarted();
    void doNotifyAdvertisingStopped();
    void doNotifyConnected();
    void doNotifyDisconnected();
    void doPowerOff();
    [[nodiscard]] BLEDefs::ErrCode doSendCommand(BleTxCmd& cmd);
    [[nodiscard]] BLEDefs::ErrCode doSendRequest(BleTxRequest& request);

private:
    blehfsm::Off offState;
    blehfsm::On onState;
    blehfsm::Started startedState;
    blehfsm::Dfu dfuState;
    blehfsm::Ready readyState;
    blehfsm::Configured configuredState;
    blehfsm::Enabled enabledState;
    blehfsm::StartingAdvertising startingAdvertisingState;
    blehfsm::Advertising advertisingState;
    blehfsm::Connected connectedState;

    etl::array<etl::ifsm_state*, blehfsm::StateId::NUMBER_OF_STATES>
        stateList{&offState,
                  &onState,
                  &startedState,
                  &dfuState,
                  &readyState,
                  &configuredState,
                  &enabledState,
                  &startingAdvertisingState,
                  &advertisingState,
                  &connectedState};
    etl::array<etl::ifsm_state*, 3> enabledChildStates{&startingAdvertisingState,
                                                       &advertisingState,
                                                       &connectedState};
    etl::array<etl::ifsm_state*, 2> readyChildStates{&configuredState, &enabledState};

    IBleCommunication& m_bleCommunication;
    BleNrfStartedCmd m_startedCmd;
    BleEventCmd m_bleEventCmd;
    BleEnteredDfuCmd m_enteredDfuCmd;

    // Device configuration commands
    // They need to be set only once and then can be resent as much as needed
    BleHidReportDescriptorCmd m_hidReportDescriptorCmd;
    BleSetMacAddressCmd m_setMacAddressCmd;
    bool m_isSetMacAddress{false};
    BleSetDeviceNameCmd m_setDeviceNameCmd;
    BleSetDevicePropertiesCmd m_setDevicePropertiesCmd;
};
