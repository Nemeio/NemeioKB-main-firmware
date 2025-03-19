#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "LedInformation.hpp"
#include "mocks/MockLED.hpp"
#include "mocks/MockTimer.hpp"
#include "KeyboardParameters.hpp"

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::StrictMock;

class LedInformationTest : public ::testing::Test
{
protected:
    LedInformationTest()
        : ledInformation(mCapsLockLed,
                         mBatteryLed,
                         mBleLed,
                         mBatteryTimer,
                         mBleTimer,
                         mUpdateTimer){};

    NiceMock<MockTimer> mBatteryTimer;
    NiceMock<MockTimer> mBleTimer;
    NiceMock<MockTimer> mUpdateTimer;

    StrictMock<MockLED> mCapsLockLed;
    StrictMock<MockLED> mBatteryLed;
    StrictMock<MockLED> mBleLed;

    LedInformation ledInformation;
};

TEST_F(LedInformationTest, NominalCycle)
{
    for(int i = 0; i < 2; ++i)
    {
        EXPECT_CALL(mBleLed, turnOn).Times(1);
        ledInformation.notification(BleEvent::ADVERTISING_STARTED);

        EXPECT_CALL(mBleLed, turnOff).Times(1);
        EXPECT_CALL(mBleLed, turnOn).Times(1);
        ledInformation.notification(BleEvent::CONNECTED);

        EXPECT_CALL(mBleLed, turnOff).Times(2);
        ledInformation.notification(BleEvent::DISCONNECTED);
    }
};

TEST_F(LedInformationTest, AdvertisingStartStop)
{
    for(int i = 0; i < 2; ++i)
    {
        EXPECT_CALL(mBleLed, turnOn).Times(1);
        ledInformation.notification(BleEvent::ADVERTISING_STARTED);

        EXPECT_CALL(mBleLed, turnOff).Times(2);
        ledInformation.notification(BleEvent::ADVERTISING_STOPPED);
    }
};

TEST_F(LedInformationTest, ConnectedDisconnected)
{
    for(int i = 0; i < 2; ++i)
    {
        EXPECT_CALL(mBleLed, turnOn).Times(1);
        ledInformation.notification(BleEvent::CONNECTED);

        EXPECT_CALL(mBleLed, turnOff).Times(2);
        ledInformation.notification(BleEvent::DISCONNECTED);
    }
};

TEST_F(LedInformationTest, AdvertisingAndConnected)
{
    EXPECT_CALL(mBleLed, turnOn).Times(1);
    ledInformation.notification(BleEvent::ADVERTISING_STARTED);

    EXPECT_CALL(mBleLed, turnOff).Times(1);
    EXPECT_CALL(mBleLed, turnOn).Times(1);
    ledInformation.notification(BleEvent::CONNECTED);
};

TEST_F(LedInformationTest, CapsLockOn)
{
    EXPECT_CALL(mCapsLockLed, turnOn).Times(1);

    Identifiable dummySource;
    HIDDefs::KeyLockStates keyLockStates;
    keyLockStates.capsLock = true;
    ledInformation.keyLockStatesChanged(dummySource, keyLockStates);
};

TEST_F(LedInformationTest, CapsLockOff)
{
    EXPECT_CALL(mCapsLockLed, turnOff).Times(1);

    Identifiable dummySource;
    HIDDefs::KeyLockStates keyLockStates;
    keyLockStates.capsLock = false;

    ledInformation.keyLockStatesChanged(dummySource, keyLockStates);
};

TEST_F(LedInformationTest, UpdateStarted)
{
    /* Disable old strategy for BLE and CapsLock and start update strategy */

    EXPECT_CALL(mCapsLockLed, turnOn).Times(1);
    EXPECT_CALL(mBleLed, turnOn).Times(1);

    ledInformation.onUpdateEvent(UpdateStatus::START);
};

TEST_F(LedInformationTest, UpdateStopped)
{
    /* Disable update strategy and restore BLE and CapsLock strategy */

    EXPECT_CALL(mBleLed, turnOff).Times(2);
    EXPECT_CALL(mCapsLockLed, turnOff).Times(2);

    ledInformation.onUpdateEvent(UpdateStatus::END);
};

TEST_F(LedInformationTest, UpdateFailed)
{
    /* Disable update strategy and restore BLE and CapsLock strategy */

    EXPECT_CALL(mBleLed, turnOff).Times(2);
    EXPECT_CALL(mCapsLockLed, turnOff).Times(2);

    ledInformation.onUpdateEvent(UpdateStatus::ERROR);
};

TEST_F(LedInformationTest, UpdateStoppedWhenBLEConnected)
{
    /* Disable update strategy and restore BLE and CapsLock strategy */

    EXPECT_CALL(mBleLed, turnOn).Times(3);
    EXPECT_CALL(mBleLed, turnOff).Times(2);
    EXPECT_CALL(mCapsLockLed, turnOn).Times(1);
    EXPECT_CALL(mCapsLockLed, turnOff).Times(2);

    ledInformation.notification(BleEvent::CONNECTED);
    ledInformation.onUpdateEvent(UpdateStatus::START);
    ledInformation.onUpdateEvent(UpdateStatus::END);
};

TEST_F(LedInformationTest, UpdateStoppedWhenCapsLocked)
{
    /* Disable update strategy and restore BLE and CapsLock strategy */

    EXPECT_CALL(mCapsLockLed, turnOn).Times(3);
    EXPECT_CALL(mCapsLockLed, turnOff).Times(2);
    EXPECT_CALL(mBleLed, turnOn).Times(1);
    EXPECT_CALL(mBleLed, turnOff).Times(2);

    Identifiable dummySource;
    HIDDefs::KeyLockStates keyLockStates;
    keyLockStates.capsLock = true;

    ledInformation.keyLockStatesChanged(dummySource, keyLockStates);
    ledInformation.onUpdateEvent(UpdateStatus::START);
    ledInformation.onUpdateEvent(UpdateStatus::END);
};

TEST_F(LedInformationTest, BatteryStates)
{
    InSequence seq;
    EXPECT_CALL(mBatteryLed, turnOff).Times(1);
    ledInformation.normalBatteryState();

    EXPECT_CALL(mBatteryLed, turnOn).Times(1);
    ledInformation.lowBatteryState();

    EXPECT_CALL(mBatteryLed, turnOff).Times(2);
    ledInformation.charging();
};

/* TODO Currently, we can't test that keyboard parameters delays are taken into account */
