#pragma once

#include "etl/vector.h"
#include <cstdint>

#include "keyIDConfig.h"

namespace configuration
{
inline constexpr size_t KEY_SEQUENCE_MAX_KEYS = 3;
using KeySequence = etl::vector<KeyIntIndex, KEY_SEQUENCE_MAX_KEYS>;
} // namespace configuration
