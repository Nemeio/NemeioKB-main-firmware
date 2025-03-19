#include "BleChip.hpp"
#include "BleSetAdvertisingCmd.hpp"
#include "assertTools.h"
#include "AutoLock.h"
#include "BleDfuStartCmd.hpp"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "ble"

using namespace blehfsm;

etl::fsm_state_id_t Off::on_event(const InitEvent&)
{
    get_fsm_context().doInit();
    return StateId::ON;
}

etl::fsm_state_id_t On::on_event(const PowerOffEvent&)
{
    get_fsm_context().doPowerOff();
    return StateId::OFF;
}

etl::fsm_state_id_t On::on_event(const StartedEvent&)
{
    return StateId::STARTED;
}

etl::fsm_state_id_t On::on_event(const EnteredDfuEvent&)
{
    return StateId::DFU;
}

etl::fsm_state_id_t Started::on_enter_state()
{
    get_fsm_context().doNotifyStarted();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Started::on_event(const PowerOffEvent&)
{
    get_fsm_context().doPowerOff();
    return StateId::OFF;
}

etl::fsm_state_id_t Started::on_event(const StartedEvent&)
{
    get_fsm_context().doNotifyStarted();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Started::on_event(const EnteredDfuEvent&)
{
    return StateId::DFU;
}

etl::fsm_state_id_t Started::on_event(const ConfigureEvent&)
{
    get_fsm_context().doConfigure();
    return StateId::READY;
}

etl::fsm_state_id_t Started::on_event(const SendApplicationCommandEvent& event)
{
    event.ret = get_fsm_context().doSendCommand(event.cmd);
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Started::on_event(const SendApplicationRequestEvent& event)
{
    event.ret = get_fsm_context().doSendRequest(event.cmd);
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Dfu::on_enter_state()
{
    get_fsm_context().doNotifyEnteredDfu();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Dfu::on_event(const PowerOffEvent&)
{
    get_fsm_context().doPowerOff();
    return StateId::OFF;
}

etl::fsm_state_id_t Dfu::on_event(const SendDfuCommandEvent& event)
{
    event.ret = get_fsm_context().doSendCommand(event.cmd);
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Ready::on_enter_state()
{
    get_fsm_context().doNotifyReady();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Ready::on_event(const PowerOffEvent&)
{
    get_fsm_context().doPowerOff();
    return StateId::OFF;
}

etl::fsm_state_id_t Ready::on_event(const EnteredDfuEvent&)
{
    return StateId::DFU;
}

etl::fsm_state_id_t Ready::on_event(const SendApplicationCommandEvent& event)
{
    event.ret = get_fsm_context().doSendCommand(event.cmd);
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Ready::on_event(const SendApplicationRequestEvent& event)
{
    event.ret = get_fsm_context().doSendRequest(event.cmd);
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Ready::on_event(const SendBleCommandEvent& event)
{
    event.ret = get_fsm_context().doSendCommand(event.cmd);
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Configured::on_event(const StartedEvent&)
{
    get_fsm_context().doConfigure();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t Configured::on_event(const EnableBleEvent&)
{
    return StateId::ENABLED;
}

void Enabled::on_exit_state()
{
    get_fsm_context().doStopAdvertising();
}

etl::fsm_state_id_t Enabled::on_event(const StartedEvent&)
{
    get_fsm_context().doConfigure();
    return StateId::STARTING_ADVERTISING;
}

etl::fsm_state_id_t Enabled::on_event(const EnteredDfuEvent&)
{
    return StateId::DFU;
}

etl::fsm_state_id_t Enabled::on_event(const DisableBleEvent&)
{
    return StateId::CONFIGURED;
}

etl::fsm_state_id_t StartingAdvertising::on_enter_state()
{
    get_fsm_context().doStartAdvertising();
    return etl::ifsm_state::No_State_Change;
}

etl::fsm_state_id_t StartingAdvertising::on_event(const AdvertisingStartedEvent&)
{
    return StateId::ADVERTISING;
}

etl::fsm_state_id_t Advertising::on_enter_state()
{
    get_fsm_context().doNotifyAdvertisingStarted();
    return etl::ifsm_state::No_State_Change;
}

void Advertising::on_exit_state()
{
    get_fsm_context().doNotifyAdvertisingStopped();
}

etl::fsm_state_id_t Advertising::on_event(const ConnectedEvent&)
{
    return StateId::CONNECTED;
}

etl::fsm_state_id_t Advertising::on_event(const AdvertisingStoppedEvent&)
{
    return StateId::STARTING_ADVERTISING;
}

etl::fsm_state_id_t Connected::on_enter_state()
{
    get_fsm_context().doNotifyConnected();
    return etl::ifsm_state::No_State_Change;
}

void Connected::on_exit_state()
{
    get_fsm_context().doNotifyDisconnected();
}

etl::fsm_state_id_t Connected::on_event(const DisconnectedEvent&)
{
    return StateId::STARTING_ADVERTISING;
}

BleChip::BleChip(IBleCommunication& bleCommunication)
    : Hfsm(BLE_HFSM)
    , m_bleCommunication(bleCommunication)
{
    enabledState.set_child_states(enabledChildStates.data(), enabledChildStates.size());
    readyState.set_child_states(readyChildStates.data(), readyChildStates.size());
    set_states(stateList.data(), stateList.size());
};

BleChip::~BleChip()
{
    unregisterBleCommandListener(m_startedCmd);
    unregisterBleCommandListener(m_bleEventCmd);
    unregisterBleCommandListener(m_enteredDfuCmd);
}

void BleChip::registerBleCommandListener(AbstractBleCommandListener& commandListener)
{
    m_bleCommunication.registerCommandListener(commandListener);
}

void BleChip::unregisterBleCommandListener(AbstractBleCommandListener& commandListener)
{
    m_bleCommunication.unregisterListener(commandListener);
}

void BleChip::setHidReportMap(etl::array_view<const uint16_t> reportLengths,
                              etl::array_view<const uint8_t> reportMap)
{
    m_hidReportDescriptorCmd.setReportLengths(reportLengths);
    m_hidReportDescriptorCmd.setReportDescriptor(reportMap);
}

void BleChip::setDeviceName(etl::string_view name)
{
    m_setDeviceNameCmd.setDeviceName(name);
}

void BleChip::setDeviceMacAddress(const MacAddress& mac)
{
    m_setMacAddressCmd.setMacAddress(mac);
    m_isSetMacAddress = true;
}

void BleChip::setDeviceProperties(const BLEDefs::bleDeviceProperties& deviceProperties)
{
    m_setDevicePropertiesCmd.setDeviceProperties(deviceProperties);
}

void BleChip::enterDfu()
{
    // To enter dfu, do not send the event to the state machine
    // The nRF will send in response an event that will trigger the state machine
    BleDfuStartCmd cmd;
    m_bleCommunication.sendCommand(cmd);
}

void BleChip::forceDfu()
{
    // To enter dfu, do not send the event to the state machine
    // The nRF will send in response an event that will trigger the state machine
    m_bleCommunication.forceDfu();
}

void BleChip::init()
{
    m_startedCmd.add_observer(*this);
    m_bleEventCmd.add_observer(*this);
    m_enteredDfuCmd.add_observer(*this);

    registerBleCommandListener(m_startedCmd);
    registerBleCommandListener(m_bleEventCmd);
    registerBleCommandListener(m_enteredDfuCmd);

    start();

    receive(blehfsm::InitEvent());
}

void BleChip::configure()
{
    receive(blehfsm::ConfigureEvent());
}

BLEDefs::ErrCode BleChip::sendCommand(BleReadyForBleTxCommandRef cmd)
{
    BLEDefs::ErrCode ret = BLEDefs::ErrCode::DEVICE_NOT_READY;

    BleTxCmd& cmdBase = castToBase<BleTxCmd>(cmd);
    receive(blehfsm::SendBleCommandEvent(cmdBase, ret));

    return ret;
}

BLEDefs::ErrCode BleChip::sendCommand(BleApplicationTxCommandRef cmd)
{
    BLEDefs::ErrCode ret = BLEDefs::ErrCode::DEVICE_NOT_READY;

    BleTxCmd& cmdBase = castToBase<BleTxCmd>(cmd);
    receive(blehfsm::SendApplicationCommandEvent(cmdBase, ret));

    return ret;
}

BLEDefs::ErrCode BleChip::sendCommand(BleDfuModeTxCommandRef cmd)
{
    BLEDefs::ErrCode ret = BLEDefs::ErrCode::DEVICE_NOT_READY;

    BleTxCmd& cmdBase = castToBase<BleTxCmd>(cmd);
    receive(blehfsm::SendDfuCommandEvent(cmdBase, ret));

    return ret;
}

BLEDefs::ErrCode BleChip::sendRequest(BleApplicationTxRequestRef cmd)
{
    BLEDefs::ErrCode ret = BLEDefs::ErrCode::DEVICE_NOT_READY;

    BleTxRequest& cmdBase = castToBase<BleTxRequest>(cmd);
    receive(blehfsm::SendApplicationRequestEvent(cmdBase, ret));

    return ret;
}

void BleChip::enableBle()
{
    receive(blehfsm::EnableBleEvent());
}

void BleChip::disableBle()
{
    receive(blehfsm::DisableBleEvent());
}

void BleChip::powerOff()
{
    receive(blehfsm::PowerOffEvent());
}

void BleChip::notification(BleEventCmdEvent event)
{
    switch(event.value)
    {
    case BleEventValue::ADVERTISING_STARTED:
        receive(blehfsm::AdvertisingStartedEvent());
        break;
    case BleEventValue::ADVERTISING_STOPPED:
        receive(blehfsm::AdvertisingStoppedEvent());
        break;
    case BleEventValue::CONNECTED:
        receive(blehfsm::ConnectedEvent());
        break;
    case BleEventValue::DISCONNECTED:
        receive(blehfsm::DisconnectedEvent());
        break;
    default:
        break;
    }
}

void BleChip::notification(const BleStartedEvent& event)
{
    LOG_INFO(MODULE, "Started");
    receive(blehfsm::StartedEvent());
}

void BleChip::notification(BleEnteredDfuEvent event)
{
    LOG_INFO(MODULE, "Entered DFU");
    receive(blehfsm::EnteredDfuEvent());
}

void BleChip::doInit()
{
    m_bleCommunication.init();
}

void BleChip::doConfigure()
{
    if(m_isSetMacAddress)
    {
        m_bleCommunication.sendCommand(m_setMacAddressCmd);
    }
    m_bleCommunication.sendCommand(m_setDeviceNameCmd);
    m_bleCommunication.sendCommand(m_setDevicePropertiesCmd);
    m_bleCommunication.sendCommand(m_hidReportDescriptorCmd);
}

void BleChip::doStartAdvertising()
{
    BleSetAdvertisingCmd cmd;
    cmd.setEnable(true);
    m_bleCommunication.sendCommand(cmd);
}

void BleChip::doStopAdvertising()
{
    BleSetAdvertisingCmd cmd;
    cmd.setEnable(false);
    m_bleCommunication.sendCommand(cmd);
}

void BleChip::doNotifyStarted()
{
    BleEventObservable::notify_observers(BleEvent::STARTED);
}

void BleChip::doNotifyReady()
{
    BleEventObservable::notify_observers(BleEvent::READY);
}

void BleChip::doNotifyEnteredDfu()
{
    BleEventObservable::notify_observers(BleEvent::ENTERED_DFU);
}

void BleChip::doNotifyAdvertisingStarted()
{
    LOG_INFO(MODULE, "Advertising started");
    BleEventObservable::notify_observers(BleEvent::ADVERTISING_STARTED);
}

void BleChip::doNotifyAdvertisingStopped()
{
    LOG_INFO(MODULE, "Advertising stopped");
    BleEventObservable::notify_observers(BleEvent::ADVERTISING_STOPPED);
}

void BleChip::doNotifyConnected()
{
    LOG_INFO(MODULE, "Connected");
    BleEventObservable::notify_observers(BleEvent::CONNECTED);
}

void BleChip::doNotifyDisconnected()
{
    LOG_INFO(MODULE, "Disconnected");
    BleEventObservable::notify_observers(BleEvent::DISCONNECTED);
}

void BleChip::doPowerOff()
{
    m_bleCommunication.powerOff();
}

BLEDefs::ErrCode BleChip::doSendCommand(BleTxCmd& cmd)
{
    return m_bleCommunication.sendCommand(cmd);
}

BLEDefs::ErrCode BleChip::doSendRequest(BleTxRequest& request)
{
    return m_bleCommunication.sendRequest(request);
}