#pragma once

#include "Observer.hpp"
#include "BLEDefs.hpp"
#include "BleTxRequestTimeout.hpp"

struct FactoryResetEvent
{
    BLEDefs::ErrCode err;
};

using BleFactoryResetCmdObserver = etl::observer<FactoryResetEvent, BleTxRequestTimeout>;