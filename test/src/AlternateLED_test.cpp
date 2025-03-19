#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AlternateLED.hpp"
#include "mocks/MockLED.hpp"
#include "mocks/MockTimer.hpp"

inline constexpr DelayMs DELAY_ON_MS = 100;
inline constexpr int NB_ALTERNATE_TEST = 3;

using ::testing::NiceMock;
using ::testing::StrictMock;

class AlternateLEDTest : public ::testing::Test
{
protected:
    AlternateLEDTest()
        : alternateLED(firstLed, secondLed, timer){};

    StrictMock<MockLED> firstLed;
    StrictMock<MockLED> secondLed;
    NiceMock<MockTimer> timer;
    AlternateLED alternateLED;
};

TEST_F(AlternateLEDTest, LEDOn)
{
    EXPECT_CALL(firstLed, turnOn).Times(1);
    EXPECT_CALL(secondLed, turnOff).Times(1);

    alternateLED.startLedStrategy();
};

TEST_F(AlternateLEDTest, LEDOnMultipleTimes)
{
    EXPECT_CALL(firstLed, turnOn).Times(2);
    EXPECT_CALL(secondLed, turnOff).Times(2);

    alternateLED.startLedStrategy();
    alternateLED.startLedStrategy();
};

TEST_F(AlternateLEDTest, LEDOff)
{
    EXPECT_CALL(firstLed, turnOn).Times(1);
    EXPECT_CALL(secondLed, turnOff).Times(1);
    alternateLED.startLedStrategy();

    EXPECT_CALL(firstLed, turnOff).Times(1);
    EXPECT_CALL(secondLed, turnOff).Times(1);

    alternateLED.stopLedStrategy();
};

TEST_F(AlternateLEDTest, LEDOffMultipleTimes)
{
    EXPECT_CALL(firstLed, turnOn).Times(1);
    EXPECT_CALL(secondLed, turnOff).Times(1);

    alternateLED.startLedStrategy();

    EXPECT_CALL(firstLed, turnOff).Times(2);
    EXPECT_CALL(secondLed, turnOff).Times(2);

    alternateLED.stopLedStrategy();
    alternateLED.stopLedStrategy();
};

TEST_F(AlternateLEDTest, LEDAlternate)
{
    EXPECT_CALL(firstLed, turnOn).Times(1);
    EXPECT_CALL(secondLed, turnOff).Times(1);
    EXPECT_CALL(timer, start).Times(1);
    alternateLED.startLedStrategy();

    for(int i = 0; i < NB_ALTERNATE_TEST; ++i)
    {
        EXPECT_CALL(firstLed, turnOff).Times(1);
        EXPECT_CALL(secondLed, turnOn).Times(1);
        EXPECT_CALL(timer, start).Times(1);
        alternateLED.timerCallback();

        EXPECT_CALL(firstLed, turnOn).Times(1);
        EXPECT_CALL(secondLed, turnOff).Times(1);
        EXPECT_CALL(timer, start).Times(1);
        alternateLED.timerCallback();
    }

    EXPECT_CALL(firstLed, turnOff).Times(1);
    EXPECT_CALL(secondLed, turnOff).Times(1);
    EXPECT_CALL(timer, stop).Times(1);
    alternateLED.stopLedStrategy();
};
