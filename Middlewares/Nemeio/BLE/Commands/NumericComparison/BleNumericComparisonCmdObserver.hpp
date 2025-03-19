#pragma once

#include "Observer.hpp"
#include "BLEDefs.hpp"

struct BleNumericComparisonEvent
{
    BLEDefs::Passkey passkey;
};

using BleNumericComparisonCmdObserver = etl::observer<const BleNumericComparisonEvent&>;