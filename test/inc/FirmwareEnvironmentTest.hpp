#pragma once

#include "utils.hpp"

namespace firmware_environment_test
{
static constexpr uint32_t LATEST_VERSION = 6;
static constexpr uint32_t FILE_SYSTEM_VERSION = 2;

PACK((struct envSettingsV4_t
      {
          FirmwareUpdateBank nextFirmwareUpdateBank;
          bool updateInProgress;
          bool electricalTestsPassed;
          bool functionalTestsPassed;
      }));

PACK((struct envSettingsV5_t
      {
          FirmwareUpdateBank nextFirmwareUpdateBank;
          bool updateInProgress;
          bool electricalTestsPassed;
          bool functionalTestsPassed;
          uint8_t fsVersion;
      }));

PACK((struct envSettingsV6_t
      {
          FirmwareUpdateBank nextFirmwareUpdateBank;
          bool updateInProgress;
          bool electricalTestsPassed;
          bool functionalTestsPassed;
          uint8_t fsVersion;
          char devTestBenchId[TestBenchId::SIZE];
          uint8_t devBleMacAddress[MacAddress::SIZE];
          char devSerialNumber[SerialNumber::SIZE];
      }));

PACK((struct envSettings_t
      {
          uint32_t version;
          union
          {
              envSettingsV4_t v4;
              envSettingsV5_t v5;
              envSettingsV6_t v6;
              envSettingsV6_t latest;
          };
      }));

} // namespace firmware_environment_test