#pragma once

#include "gmock/gmock.h"
#include "IKeyboardStatus.hpp"

class MockKeyboardStatus : public IKeyboardStatus
{
public:
    MOCK_METHOD(void, refresh, (), (override));
    MOCK_METHOD(size_t, getLogSize, (), (const override));
    MOCK_METHOD(bool, isErrorLogged, (), (const override));
};