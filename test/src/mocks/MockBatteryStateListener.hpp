#ifndef MOCKBATTERYSTATELISTENER_HPP_
#define MOCKBATTERYSTATELISTENER_HPP_

#include "gmock/gmock.h"
#include "IBatteryStateListener.hpp"

class MockBatteryStateListener : public IBatteryStateListener
{
public:
    MOCK_METHOD(void, lowBatteryState, (), (override));
    MOCK_METHOD(void, normalBatteryState, (), (override));
    MOCK_METHOD(void, charging, (), (override));
};

#endif /* MOCKBATTERYSTATELISTENER_HPP_ */
