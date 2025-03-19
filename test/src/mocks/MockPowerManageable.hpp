#ifndef MOCKS_MOCKPOWERMANAGEABLE_HPP_
#define MOCKS_MOCKPOWERMANAGEABLE_HPP_

#include "gmock/gmock.h"
#include "IPowerManageable.h"

class MockPowerManageable : public IPowerManageable
{
public:
    MOCK_METHOD(void, active, (), (override));
    MOCK_METHOD(void, inactive, (), (override));
    MOCK_METHOD(void, longInactive, (), (override));
    MOCK_METHOD(void, sleep, (), (override));
    MOCK_METHOD(void, powerOff, (), (override));
    MOCK_METHOD(void, suspend, (), (override));
};

#endif /* MOCKS_MOCKPOWERMANAGEABLE_HPP_ */
