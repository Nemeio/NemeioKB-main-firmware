#include "gtest/gtest.h"

#include "PowerStateMachine.hpp"
#include "MockPowerController.hpp"
#include "MockPowerManageable.hpp"
#include "ListenerCollection.hpp"

using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;

class PowerStateMachineTest : public ::testing::Test
{
protected:
    PowerStateMachineTest()
        : listeners({&listener})
        , context(powerController, listeners){};

    void initPluggedIn()
    {
        context.updateTimeouts(testTimeouts);
        context.updatePowerConnectionState(PLUG_IN_PC);
        EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
    }

    StrictMock<MockPowerController> powerController;
    StrictMock<MockPowerManageable> listener;
    ListenerCollection<IPowerManageable*, 1> listeners;
    powerfsm::PowerStateMachine context;

    static constexpr DelaySec ACTIVE_TIMEOUT = DelaySec(60);
    static constexpr DelaySec INACTIVE_TIMEOUT = DelaySec(120);
    static constexpr DelaySec LONG_INACTIVE_TIMEOUT = DelaySec(10);
    static constexpr DelaySec ACTIVE_TIMEOUT_USB_DISCONNECTED = DelaySec(30);
    static constexpr DelaySec INACTIVE_TIMEOUT_USB_DISCONNECTED = DelaySec(35);
    static constexpr DelaySec LONG_INACTIVE_TIMEOUT_USB_DISCONNECTED = DelaySec(15);
    static constexpr DelaySec SLEEP_TIMEOUT_USB_DISCONNECTED = DelaySec(40);

    powerfsm::Timeouts testTimeouts = powerfsm::Timeouts(ACTIVE_TIMEOUT,
                                                         INACTIVE_TIMEOUT,
                                                         LONG_INACTIVE_TIMEOUT,
                                                         ACTIVE_TIMEOUT_USB_DISCONNECTED,
                                                         INACTIVE_TIMEOUT_USB_DISCONNECTED,
                                                         LONG_INACTIVE_TIMEOUT_USB_DISCONNECTED,
                                                         SLEEP_TIMEOUT_USB_DISCONNECTED);
};

TEST_F(PowerStateMachineTest, InitPluggedIn)
{
    initPluggedIn();
}

TEST_F(PowerStateMachineTest, InitTimeoutsSetAfterUSBState)
{
    context.updatePowerConnectionState(PLUG_IN_PC);
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.updateTimeouts(testTimeouts);
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, AlreadyActive)
{
    initPluggedIn();

    EXPECT_CALL(listener, active).Times(0);
    context.userActivityDetected();
}

TEST_F(PowerStateMachineTest, ActivityCycleUSBConnected)
{
    initPluggedIn();

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);

    EXPECT_CALL(listener, inactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), INACTIVE_TIMEOUT);

    EXPECT_CALL(listener, longInactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), LONG_INACTIVE_TIMEOUT);

    EXPECT_CALL(listener, sleep).Times(1);
    EXPECT_CALL(powerController, sleep).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    EXPECT_CALL(listener, active).Times(1);
    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, ActivityCycleUSBConnectedPowerSupplyOnly)
{
    initPluggedIn();

    context.updatePowerConnectionState(PLUG_IN_POWER_SUPPLY_ONLY);
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);

    EXPECT_CALL(listener, inactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), INACTIVE_TIMEOUT);

    EXPECT_CALL(listener, longInactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), LONG_INACTIVE_TIMEOUT);

    EXPECT_CALL(listener, sleep).Times(1);
    EXPECT_CALL(powerController, sleep).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    EXPECT_CALL(listener, active).Times(1);
    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, ActivityCycleUSBConnectedActiveFromInactive)
{
    initPluggedIn();

    EXPECT_CALL(listener, inactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), INACTIVE_TIMEOUT);

    EXPECT_CALL(listener, active).Times(1);
    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, ActivityCycleUSBDisconnected)
{
    initPluggedIn();

    context.updatePowerConnectionState(PLUG_OUT);
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT_USB_DISCONNECTED);

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, inactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), INACTIVE_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, longInactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), LONG_INACTIVE_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, sleep).Times(1);
    EXPECT_CALL(powerController, sleep).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), SLEEP_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, powerOff).Times(1);
    EXPECT_CALL(powerController, shutdown).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));
}

TEST_F(PowerStateMachineTest, ActivityCycleUSBDisconnectedActiveFromSleep)
{
    initPluggedIn();

    context.updatePowerConnectionState(PLUG_OUT);
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT_USB_DISCONNECTED);

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, inactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), INACTIVE_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, longInactive).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), LONG_INACTIVE_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, sleep).Times(1);
    EXPECT_CALL(powerController, sleep).Times(1);
    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), SLEEP_TIMEOUT_USB_DISCONNECTED);

    EXPECT_CALL(listener, active).Times(1);
    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT_USB_DISCONNECTED);
}

TEST_F(PowerStateMachineTest, DemoMode)
{
    initPluggedIn();

    context.startDemo();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    EXPECT_CALL(listener, active).Times(1);
    context.stopDemo();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, SuspendExitFromActivity)
{
    initPluggedIn();

    EXPECT_CALL(listener, suspend).Times(1);
    context.updatePowerConnectionState(PLUG_IN_PC_SUSPENDED);
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    EXPECT_CALL(listener, active).Times(1);
    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, SuspendExitFromUSBWakeUp)
{
    initPluggedIn();

    EXPECT_CALL(listener, suspend).Times(1);
    context.updatePowerConnectionState(PLUG_IN_PC_SUSPENDED);
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    EXPECT_CALL(listener, active).Times(1);
    context.updatePowerConnectionState(PLUG_IN_PC);
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT);
}

TEST_F(PowerStateMachineTest, SuspendExitFromUSBPlugOut)
{
    initPluggedIn();

    EXPECT_CALL(listener, suspend).Times(1);
    context.updatePowerConnectionState(PLUG_IN_PC_SUSPENDED);
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    EXPECT_CALL(listener, active).Times(1);
    context.updatePowerConnectionState(PLUG_OUT);
    EXPECT_EQ(context.getCurrentTimeout(), ACTIVE_TIMEOUT_USB_DISCONNECTED);
}

TEST_F(PowerStateMachineTest, PowerOff)
{
    initPluggedIn();

    {
        InSequence s;

        // Listeners must be notified before the shutdown happens
        EXPECT_CALL(listener, powerOff).Times(1);
        EXPECT_CALL(powerController, shutdown).Times(1);
        context.powerOff();
    }

    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.timeout();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));

    context.userActivityDetected();
    EXPECT_EQ(context.getCurrentTimeout(), DelaySec(DelaySec::INFINITE_DELAY));
}
