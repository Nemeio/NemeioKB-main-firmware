#ifndef MOCKS_MOCKBLEEVENTLISTENER_HPP_
#define MOCKS_MOCKBLEEVENTLISTENER_HPP_

#include "gmock/gmock.h"
#include "BleEventObserver.hpp"

class MockBLEEventListener : public BleEventObserver
{
public:
    MOCK_METHOD(void, bleAdvertisingStarted, (), (override));
    MOCK_METHOD(void, bleAdvertisingStopped, (), (override));
    MOCK_METHOD(void, bleConnected, (), (override));
    MOCK_METHOD(void, bleDisconnected, (), (override));
};

#endif /* MOCKS_MOCKBLEEVENTLISTENER_HPP_ */