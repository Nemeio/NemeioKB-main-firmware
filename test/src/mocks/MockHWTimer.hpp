#ifndef MOCKS_MOCKHWTIMER_HPP_
#define MOCKS_MOCKHWTIMER_HPP_

#include "gmock/gmock.h"
#include "AbstractHWTimer.hpp"

class MockHWTimer : public AbstractHWTimer
{
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(RetCode, setDelay, (const DelayMs& delay), (override));
    MOCK_METHOD(RetCode, setDelayUs, (const DelayUs& delay), (override));
};

#endif /* MOCKS_MOCKHWTIMER_HPP_ */