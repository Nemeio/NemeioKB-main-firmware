#ifndef MOCKS_MOCKGPIOCONTROLLER_HPP_
#define MOCKS_MOCKGPIOCONTROLLER_HPP_

#include "gmock/gmock.h"
#include "IGPIOController.hpp"

class MockGPIOController : public IGPIOController
{
public:
    MOCK_METHOD(void, configureOutput, (GPIOPin gpioPin, GPIOOutConfig gpioOutConfig), (override));
    MOCK_METHOD(void, configureInput, (GPIOPin gpioPin, GPIOInConfig gpioInConfig), (override));

    MOCK_METHOD(GPIOPinState, read, (GPIOPin gpioPin), (override));
    MOCK_METHOD(void, set, (GPIOPin gpioPin), (override));
    MOCK_METHOD(void, clear, (GPIOPin gpioPin), (override));
    MOCK_METHOD(void, deinit, (GPIOPin gpioPin), (override));
};

#endif /* MOCKS_MOCKGPIOCONTROLLER_HPP_ */