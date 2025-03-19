#ifndef MOCKS_MOCKLED_HPP_
#define MOCKS_MOCKLED_HPP_

#include "gmock/gmock.h"
#include "ILED.hpp"

class MockLED : public ILED
{
public:
    MOCK_METHOD(void, turnOn, (), (override));
    MOCK_METHOD(void, turnOff, (), (override));
};

#endif /* MOCKS_MOCKLED_HPP_ */