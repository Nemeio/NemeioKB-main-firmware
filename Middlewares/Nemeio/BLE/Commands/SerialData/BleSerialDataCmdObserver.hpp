#pragma once

#include "Observer.hpp"
#include "etl/array_view.h"

struct BleSerialDataEvent
{
    etl::array_view<const uint8_t> data;
};

using BleSerialDataCmdObserver = etl::observer<const BleSerialDataEvent&>;