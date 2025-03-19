#ifndef MOCKS_MOCKPOWERCONTROLLER_HPP_
#define MOCKS_MOCKPOWERCONTROLLER_HPP_

#include "gmock/gmock.h"
#include "IPowerController.hpp"

class MockPowerController : public IPowerController
{
public:
    MOCK_METHOD(void, sleep, (), (override));
    MOCK_METHOD(void, shutdown, (), (override));
};

#endif /* MOCKS_MOCKPOWERCONTROLLER_HPP_ */