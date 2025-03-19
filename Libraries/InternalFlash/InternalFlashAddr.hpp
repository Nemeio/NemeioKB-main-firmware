#pragma once

#include "utils.hpp"

namespace internal_flash_addr
{
inline constexpr uint32_t MAIN_MEM_START = 0x08000000;
inline constexpr uint32_t MAIN_MEM_SIZE = MO_TO_BYTES(1);
inline constexpr uint32_t OTP_AREA_START = 0x1FFF7000;
inline constexpr size_t OTP_AREA_SIZE_BYTES = KO_TO_BYTES(1);
inline constexpr size_t OTP_AREA_END = OTP_AREA_START + OTP_AREA_SIZE_BYTES - 1;


} // namespace internal_flash_addr