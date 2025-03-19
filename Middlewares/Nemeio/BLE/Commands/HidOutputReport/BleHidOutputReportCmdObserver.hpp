#pragma once

#include <cstdint>
#include "Observer.hpp"

struct BleHidOutputReportEvent
{
    etl::array_view<const uint8_t> data;
};

using BleHidOutputReportCmdObserver = etl::observer<const BleHidOutputReportEvent&>;