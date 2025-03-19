#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "BlinkingLED.hpp"
#include "mocks/MockLED.hpp"
#include "mocks/MockTimer.hpp"

inline constexpr DelayMs DELAY_ON_MS = 100;
inline constexpr int32_t DELAY_OFF_MS = 200;
inline constexpr int NB_BLINK_TEST = 3;

using ::testing::NiceMock;
using ::testing::StrictMock;

class BlinkingLEDTest : public ::testing::Test
{
protected:
    BlinkingLEDTest()
        : blinkingLED(led, timer, DELAY_ON_MS, DELAY_OFF_MS){};

    StrictMock<MockLED> led;
    NiceMock<MockTimer> timer;
    BlinkingLED blinkingLED;
};

TEST_F(BlinkingLEDTest, LEDOn)
{
    EXPECT_CALL(led, turnOn).Times(1);

    blinkingLED.startLedStrategy();
};

TEST_F(BlinkingLEDTest, LEDOnMultipleTimes)
{
    EXPECT_CALL(led, turnOn).Times(2);

    blinkingLED.startLedStrategy();
    blinkingLED.startLedStrategy();
};

TEST_F(BlinkingLEDTest, LEDOff)
{
    EXPECT_CALL(led, turnOn).Times(1);
    blinkingLED.startLedStrategy();

    EXPECT_CALL(led, turnOff).Times(1);

    blinkingLED.stopLedStrategy();
};

TEST_F(BlinkingLEDTest, LEDOffMultipleTimes)
{
    EXPECT_CALL(led, turnOn).Times(1);
    blinkingLED.startLedStrategy();

    EXPECT_CALL(led, turnOff).Times(1);

    blinkingLED.stopLedStrategy();
    blinkingLED.stopLedStrategy();
};

TEST_F(BlinkingLEDTest, LEDBlink)
{
    EXPECT_CALL(led, turnOn).Times(1);
    EXPECT_CALL(timer, setDelay(DelayMs(DELAY_ON_MS))).Times(1);
    EXPECT_CALL(timer, start).Times(1);
    blinkingLED.startLedStrategy();

    for(int i = 0; i < NB_BLINK_TEST; ++i)
    {
        EXPECT_CALL(led, turnOff).Times(1);
        EXPECT_CALL(timer, setDelay(DelayMs(DELAY_OFF_MS))).Times(1);
        EXPECT_CALL(timer, start).Times(1);
        blinkingLED.timerCallback();

        EXPECT_CALL(led, turnOn).Times(1);
        EXPECT_CALL(timer, setDelay(DelayMs(DELAY_ON_MS))).Times(1);
        EXPECT_CALL(timer, start).Times(1);
        blinkingLED.timerCallback();
    }

    EXPECT_CALL(led, turnOff).Times(1);
    EXPECT_CALL(timer, stop).Times(1);
    blinkingLED.stopLedStrategy();
};
