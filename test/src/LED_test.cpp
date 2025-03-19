#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "LED.hpp"
#include "mocks/MockGPIOController.hpp"

class LEDTest : public ::testing::Test
{
protected:
    LEDTest()
        : led(gpioController, dummyPin){};

    GPIOPin dummyPin;
    MockGPIOController gpioController;
    LED led;
};

TEST_F(LEDTest, LEDOn)
{
    EXPECT_CALL(gpioController, set(dummyPin)).Times(1);

    led.turnOn();
};

TEST_F(LEDTest, LEDOff)
{
    EXPECT_CALL(gpioController, clear(dummyPin)).Times(1);

    led.turnOff();
};