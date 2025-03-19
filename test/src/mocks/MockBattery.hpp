#ifndef MOCKBATTERY_HPP_
#define MOCKBATTERY_HPP_

#include "gmock/gmock.h"
#include "IBattery.hpp"

class MockBattery : public IBattery
{
public:
    MOCK_METHOD(RetCode,
                getBatteryInformation,
                (BatteryInformation & batteryInformation),
                (override));
};

#endif /* MOCKBATTERY_HPP_ */
