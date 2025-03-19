#pragma once

#include "Observer.hpp"
#include "BLEDefs.hpp"
#include "BleTxRequestTimeout.hpp"

struct BleFwVersionEvent
{
    BLEDefs::FirmwareVersion version;
};

using BleFwVersionObserver = etl::observer<const BleFwVersionEvent&, BleTxRequestTimeout>;