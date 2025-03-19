#pragma once

#include <cstdint>
#include "Observer.hpp"

struct BleStartedEvent
{
    uint8_t reason;
};

using BleStartedCmdObserver = etl::observer<const BleStartedEvent&>;