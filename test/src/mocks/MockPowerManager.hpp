#ifndef MOCKS_MOCKPOWERMANAGER_HPP_
#define MOCKS_MOCKPOWERMANAGER_HPP_

#include "gmock/gmock.h"
#include "IPowerManager.hpp"

class MockPowerManager : public IPowerManager
{
public:
    MOCK_METHOD(void, powerOff, (), (override));
    MOCK_METHOD(void, userActivityHappened, (), (const override));
    MOCK_METHOD(void, systemActivityHappened, (), (const override));
};

#endif /* MOCKS_MOCKPOWERMANAGER_HPP_ */
