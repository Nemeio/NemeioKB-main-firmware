#pragma once

#include "etl/observer.h"

enum class BleEvent
{
    STARTED,
    READY,
    ENTERED_DFU,
    ADVERTISING_STARTED,
    ADVERTISING_STOPPED,
    CONNECTED,
    DISCONNECTED
};

using BleEventObserver = etl::observer<BleEvent>;