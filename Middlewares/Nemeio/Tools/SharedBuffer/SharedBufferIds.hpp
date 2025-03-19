#pragma once

#include <cstdint>

namespace shared_buffer_id
{
// Id 0: buffer size = 1024
inline constexpr uint8_t KEYBOARD_PARAMETERS_SHARED_BUFFER_ID = 0;

// Id 1 : buffer size = 4096
inline constexpr uint8_t CONFIGURATION_SHARED_BUFFER_ID = 1;
inline constexpr uint8_t PKG_DIGEST_COMPUTE_SHARED_BUFFER_ID = 1;
}; // namespace shared_buffer_id
