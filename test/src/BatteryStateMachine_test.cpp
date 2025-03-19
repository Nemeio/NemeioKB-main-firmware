#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "BatteryStateMachine.hpp"
#include "mocks/MockBatteryStateListener.hpp"

using ::testing::StrictMock;

class BatteryStateMachineTest : public ::testing::Test
{
protected:
    BatteryStateMachineTest()
        : batteryStateMachine(batteryStateListener)
    {
        batteryStateMachine.setLowBatteryLevelThresholdPercent(LOW_BATTERY_LEVEL_PERCENT);
    };

    StrictMock<MockBatteryStateListener> batteryStateListener;
    BatteryStateMachine batteryStateMachine;

    static constexpr uint8_t LOW_BATTERY_LEVEL_PERCENT = 20;
    static constexpr uint8_t NORMAL_BATTERY_LEVEL_PERCENT = 21;
};

TEST_F(BatteryStateMachineTest, Init)
{
    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(0);
    batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);
};

TEST_F(BatteryStateMachineTest, ChargingPluggedInComputer)
{
    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInComputer();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(0);
    batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();
};

TEST_F(BatteryStateMachineTest, ChargingPluggedInPowerSupplyOnly)
{
    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInPowerSupplyOnly();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(0);
    batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();
};

TEST_F(BatteryStateMachineTest, ChargingPluggedInPowerSuspended)
{
    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInComputerSuspended();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(0);
    batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();
};

TEST_F(BatteryStateMachineTest, ChargingPluggedInMultiple)
{
    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInComputer();
    EXPECT_CALL(batteryStateListener, charging).Times(0);
    batteryStateMachine.usbPluggedInComputer();
    batteryStateMachine.usbPluggedInComputerSuspended();
    batteryStateMachine.usbPluggedInPowerSupplyOnly();
};

TEST_F(BatteryStateMachineTest, PluggedOut)
{
    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();

    for(int i = 0; i < 3; ++i)
    {
        EXPECT_CALL(batteryStateListener, lowBatteryState).Times(1);
        batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

        EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
        batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

        EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
        batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);

        EXPECT_CALL(batteryStateListener, normalBatteryState).Times(0);
        batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);
    }
};

TEST_F(BatteryStateMachineTest, PluggedOutLowBatteryFromInit)
{
    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();
};

TEST_F(BatteryStateMachineTest, PluggedOutLowBatteryFromPluggedIn)
{
    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInComputer();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();
};

TEST_F(BatteryStateMachineTest, PluggedOutAndIn)
{
    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();

    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInComputer();
};

TEST_F(BatteryStateMachineTest, RepluggedOutWithoutBatteryLevelUpdate)
{
    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(1);
    batteryStateMachine.batteryLevel(LOW_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, charging).Times(1);
    batteryStateMachine.usbPluggedInComputer();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();
};

TEST_F(BatteryStateMachineTest, UpdateBatteryLowLevelThreshold)
{
    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(1);
    batteryStateMachine.usbPluggedOut();

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(0);
    EXPECT_CALL(batteryStateListener, normalBatteryState).Times(0);
    batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);

    batteryStateMachine.setLowBatteryLevelThresholdPercent(NORMAL_BATTERY_LEVEL_PERCENT);

    EXPECT_CALL(batteryStateListener, lowBatteryState).Times(1);
    batteryStateMachine.batteryLevel(NORMAL_BATTERY_LEVEL_PERCENT);
};
