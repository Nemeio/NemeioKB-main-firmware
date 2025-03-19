#pragma once

#include <cstdint>

inline constexpr size_t QSPI_FLASH_SIZE = MO_TO_BYTES(32);
inline constexpr size_t MINIMUM_PARTITION_SIZE = (4 * 1024);

/* Every changes in partitioning should be consider carrefully.
 * For example, you need to format LFS partition to avoid accessing block that are not available in the new mapping.
 */
PACK((struct flashPartition
      {
          uint8_t FIRMWARE_ENV_BANK_A[MINIMUM_PARTITION_SIZE];
          uint8_t FIRMWARE_ENV_BANK_B[MINIMUM_PARTITION_SIZE];
          uint8_t FIRMWARE_UPDATE_BANK_A[MO_TO_BYTES(1)];
          uint8_t FIRMWARE_UPDATE_BANK_B[MO_TO_BYTES(1)];
          uint8_t MAIN[MO_TO_BYTES(30) - (2 * MINIMUM_PARTITION_SIZE)];
      }));

static_assert(sizeof(flashPartition) == QSPI_FLASH_SIZE);

#define PARTITION_INFORMATION(partname) \
    offsetof(flashPartition, partname), member_size(flashPartition, partname)