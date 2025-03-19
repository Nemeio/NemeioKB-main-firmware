#ifndef MOCKS_MOCKTIMER_HPP_
#define MOCKS_MOCKTIMER_HPP_

#include "gmock/gmock.h"
#include "ITimer.hpp"

class MockTimer : public ITimer
{
public:
    MOCK_METHOD(void, registerSingleListener, (ITimerListener & listener), (override));
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(RetCode, setDelay, (const DelayMs& delay), (override));
};

#endif /* MOCKS_MOCKTIMER_HPP_ */