#ifndef MOCKS_MOCKSIGNAL_HPP_
#define MOCKS_MOCKSIGNAL_HPP_

#include "gmock/gmock.h"
#include "ISignal.hpp"

class MockSignal : public ISignal
{
public:
    MOCK_METHOD(bool, wait, (DelayMs delay), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(void, setFromISR, (), (override));
    MOCK_METHOD(void, set, (DelayMs delay), (override));
    MOCK_METHOD(void, set, (), (override));
};


#endif /* MOCKS_MOCKSIGNAL_HPP_ */
