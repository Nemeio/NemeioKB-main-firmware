#pragma once

#include "etl/observer.h"
#include "TestBenchStatus.hpp"

namespace benchmark
{
using ExitElectricalTestsCmdObserver = etl::observer<const TestBenchStatus&>;

} // namespace benchmark
