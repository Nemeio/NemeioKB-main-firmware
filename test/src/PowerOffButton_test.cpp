#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "PowerOffButton.hpp"
#include "MockPowerManager.hpp"
#include "MockGPIOController.hpp"
#include "MockSystem.hpp"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SaveArg;
using ::testing::StrictMock;

class PowerOffButtonTest : public ::testing::Test
{
protected:
    PowerOffButtonTest()
        : powerOffButton(powerManager, gpioController, system)
    {
        GPIOInConfig gpioInConfigCopy;
        EXPECT_CALL(gpioController, configureInput).Times(1).WillOnce(SaveArg<1>(&gpioInConfigCopy));
        powerOffButton.init();

        powerOffButtonPressCallback = gpioInConfigCopy.itCallback;

        ON_CALL(system, delayToTicks)
            .WillByDefault([](const DelayMs& delay) { return delay.getMs(); });
    };

    void buttonEvent(uint32_t tick, bool bPressed)
    {
        ASSERT_TRUE(powerOffButtonPressCallback != nullptr);

        EXPECT_CALL(gpioController, read)
            .Times(1)
            .WillOnce(Return(bPressed ? IGPIO_STATE_LOW : IGPIO_STATE_HIGH));
        EXPECT_CALL(system, getTickCount).Times(1).WillOnce(Return(tick));
        powerOffButtonPressCallback();
    }

    void pressButton(uint32_t tick) { buttonEvent(tick, true); }

    void releaseButton(uint32_t tick) { buttonEvent(tick, false); }

    StrictMock<MockPowerManager> powerManager;
    MockGPIOController gpioController;
    NiceMock<MockSystem> system;
    PowerOffButton powerOffButton;
    GPIOIntrCallback powerOffButtonPressCallback;

    static constexpr uint32_t TEST_TICK_START = 100;
};

TEST_F(PowerOffButtonTest, Nominal)
{
    uint32_t currentTick = TEST_TICK_START;

    for(int i = 0; i < 3; ++i)
    {
        pressButton(currentTick);

        currentTick += PowerOffButton::POWER_OFF_PRESS_DETECT_MIN_DELAY_MS;

        EXPECT_CALL(powerManager, powerOff).Times(1);
        releaseButton(currentTick);
    }
};

TEST_F(PowerOffButtonTest, ShortPressAndNormalPress)
{
    uint32_t currentTick = TEST_TICK_START;
    pressButton(currentTick);

    currentTick += PowerOffButton::POWER_OFF_PRESS_DETECT_MIN_DELAY_MS - 1;

    EXPECT_CALL(powerManager, powerOff).Times(0);
    releaseButton(currentTick);

    pressButton(currentTick);

    currentTick += PowerOffButton::POWER_OFF_PRESS_DETECT_MIN_DELAY_MS;

    EXPECT_CALL(powerManager, powerOff).Times(1);
    releaseButton(currentTick);
};

TEST_F(PowerOffButtonTest, FirstEventButtonReleased)
{
    uint32_t currentTick = TEST_TICK_START;
    releaseButton(currentTick);

    currentTick += PowerOffButton::POWER_OFF_PRESS_DETECT_MIN_DELAY_MS;
    EXPECT_CALL(powerManager, powerOff).Times(0);
    pressButton(currentTick);

    EXPECT_CALL(powerManager, powerOff).Times(1);

    currentTick += PowerOffButton::POWER_OFF_PRESS_DETECT_MIN_DELAY_MS;
    releaseButton(currentTick);
};