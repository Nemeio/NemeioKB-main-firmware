#pragma once

#include "Observer.hpp"

struct BleEnteredDfuEvent
{
};

using BleEnteredDfuCmdObserver = etl::observer<BleEnteredDfuEvent>;