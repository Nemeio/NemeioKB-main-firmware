#pragma once

#include "gmock/gmock.h"
#include "IDoubleClickKeyListener.hpp"

class MockDoubleClickKeyListener : public IDoubleClickKeyListener
{
public:
    MOCK_METHOD(void, simpleClicked, (const ButtonID& button), (override));
    MOCK_METHOD(void, doubleClicked, (const ButtonID& button), (override));
};