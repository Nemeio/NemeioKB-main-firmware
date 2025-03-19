#pragma once

#include "Observer.hpp"

enum class BleEventValue : uint8_t
{
    ADVERTISING_STARTED = 0,
    ADVERTISING_STOPPED = 1,
    CONNECTED = 2,
    DISCONNECTED = 3
};

struct BleEventCmdEvent
{
    BleEventValue value;
};

using BleEventCmdObserver = etl::observer<BleEventCmdEvent>;