#ifndef MOCKS_MOCKMUTEX_HPP_
#define MOCKS_MOCKMUTEX_HPP_

#include "gmock/gmock.h"
#include "AbstractMutex.hpp"

class MockMutex : public AbstractMutex
{
public:
    MOCK_METHOD(bool, unlock, (), (override));
    MOCK_METHOD(bool, doLock, (const DelayMs& timeout), (override));
};

#endif /* MOCKS_MOCKMUTEX_HPP_ */
