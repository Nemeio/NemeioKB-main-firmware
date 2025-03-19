#pragma once

#include <cstdint>
#include "Observer.hpp"

struct BlePairingAbortEvent
{
};

using BlePairingAbortCmdObserver = etl::observer<BlePairingAbortEvent>;