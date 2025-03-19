#ifndef MOCKS_MOCKSTM32HWTIMERCONTROLLER_HPP_
#define MOCKS_MOCKSTM32HWTIMERCONTROLLER_HPP_

#include "gmock/gmock.h"
#include "ISTM32HWTimerController.hpp"

class MockSTM32HWTimerController : public ISTM32HWTimerController
{
public:
    MOCK_METHOD(bool, init, (uint32_t period, uint32_t freq), (override));
    MOCK_METHOD(bool, start, (), (override));
    MOCK_METHOD(bool, stop, (), (override));
    MOCK_METHOD(void, irqHandler, (), (override));
    MOCK_METHOD(STM32HWTimerDefs::TimerId, getTimerId, (), (override));
};

#endif /* MOCKS_MOCKSTM32HWTIMERCONTROLLER_HPP_ */