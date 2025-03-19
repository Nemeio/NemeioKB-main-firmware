#pragma once

#include "gmock/gmock.h"
#include "iNemeioKeypadPressedListener.h"

class MockNemeioKeypadPressedListener : public iNemeioKeypadPressedListener
{
public:
    MOCK_METHOD(void, pressedKey, (const ButtonID& button), (override));
    MOCK_METHOD(void, unpressedKey, (const ButtonID& button), (override));
    MOCK_METHOD(void, terminateUpdateKeys, (), (override));
};
