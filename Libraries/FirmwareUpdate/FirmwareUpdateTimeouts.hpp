#pragma once

namespace fwupdatetimeouts
{
inline constexpr uint32_t ROLLBACK_WATCHDOG_TIMEOUT_SECONDS = 17;

inline constexpr uint32_t FIRMWARE_PACKAGE_STREAM_TIMEOUT_MS = 12000;
static_assert((FIRMWARE_PACKAGE_STREAM_TIMEOUT_MS + 500)
              < (ROLLBACK_WATCHDOG_TIMEOUT_SECONDS * 1000));

inline constexpr uint32_t NRF_WAIT_ENTERED_DFU_TIMEOUT_MS = 5000;
inline constexpr uint32_t NRF_NB_PING_TO_CHECK_DFU_MODE = 1;
inline constexpr uint32_t NRF_DFU_PACKET_TIMEOUT_MS = 1000;
inline constexpr uint32_t NRF_NB_DFU_PACKET_MAX_TIMEOUT = 3;

inline constexpr uint32_t NRF_MAX_PING_DURATION = NRF_NB_PING_TO_CHECK_DFU_MODE
                                                  * NRF_DFU_PACKET_TIMEOUT_MS
                                                  * NRF_NB_DFU_PACKET_MAX_TIMEOUT;

// To enter DFU, count enterDfu delay + forceDfu delay
static_assert(((NRF_WAIT_ENTERED_DFU_TIMEOUT_MS + NRF_MAX_PING_DURATION) * 2 + 500)
              < (ROLLBACK_WATCHDOG_TIMEOUT_SECONDS * 1000));

// 2 pings can happen during update on manifest reception
static_assert((NRF_WAIT_ENTERED_DFU_TIMEOUT_MS + (NRF_MAX_PING_DURATION * 2) + 500)
              < FIRMWARE_PACKAGE_STREAM_TIMEOUT_MS);
} // namespace fwupdatetimeouts