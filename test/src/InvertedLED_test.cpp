#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "InvertedLED.hpp"
#include "mocks/MockGPIOController.hpp"

class InvertedLEDTest : public ::testing::Test
{
protected:
    InvertedLEDTest()
        : led(gpioController, dummyPin){};

    GPIOPin dummyPin = PIN_UNDEFINED;
    MockGPIOController gpioController;
    InvertedLED led;
};

TEST_F(InvertedLEDTest, LEDOn)
{
    EXPECT_CALL(gpioController, clear(dummyPin)).Times(1);

    led.turnOn();
};

TEST_F(InvertedLEDTest, LEDOff)
{
    EXPECT_CALL(gpioController, set(dummyPin)).Times(1);

    led.turnOff();
};