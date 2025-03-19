/*
 * FirmwareUpdateEnvironment.hpp
 *
 *  Created on: 27 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_BANK_FIRMWAREENVIRONMENT_HPP_
#define NEMEIO_FIRMWAREUPDATE_BANK_FIRMWAREENVIRONMENT_HPP_

#include <cstdint>
#include <cstddef>
#include "fs.hpp"
#include "cpp_helper.hpp"
#include "etl/array_view.h"
#include "TestBenchId.hpp"
#include "SerialNumber.hpp"
#include "MacAddress.hpp"

enum class FirmwareUpdateBank : uint8_t
{
    UNKNOWN,
    BANK_A,
    BANK_B,
};

class FirmwareEnvironment
{
public:
    explicit FirmwareEnvironment(etl::array_view<std::byte> env);
    FirmwareEnvironment();
    virtual ~FirmwareEnvironment() = default;

    void setPendingUpdate(bool pending);
    bool isPendingUpdate();

    /* Bank to write next update */
    FirmwareUpdateBank getNextFirmwareUpdateBank();
    void setNextFirmwareUpdateBank(FirmwareUpdateBank bank);

    bool hasPassedElectricalTests();
    void passedElectricalTests(bool passed);

    bool hasPassedFunctionalTests();
    void passedFunctionalTests(bool passed);

    uint8_t getFileSystemVersion();
    void setFileSystemVersion(uint8_t fsVersion);

#ifdef DEVICE_PROPERTIES_DEBUG_STORAGE
    TestBenchId getDevTestBenchId() const;
    void setDevTestBenchId(const TestBenchId& testBenchId);

    MacAddress getDevBleMacAddress() const;
    void setDevBleMacAddress(const MacAddress& macAddress);

    SerialNumber getDevSerialNumber() const;
    void setDevSerialNumber(const SerialNumber& serialNumber);
#endif

    [[nodiscard]] size_t toBytes(etl::array_view<std::byte> env) const;

    COPYABLE(FirmwareEnvironment, delete);
    MOVABLE(FirmwareEnvironment, default);

private:
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
              // testBenchId, bleMacAddress and serialNumber fields are set and used in development mode only.
              // In production mode, device properties are stored in OTP.
              // Keep these fields in production mode to avoid managing too much envSettings versions.
              char devTestBenchId[TestBenchId::SIZE];
              uint8_t devBleMacAddress[MacAddress::SIZE];
              char devSerialNumber[SerialNumber::SIZE];
          }));

    static constexpr uint32_t VERSION = 6;
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

    envSettings_t mEnvSettings;

    void setDefaultSettings(envSettings_t& settings);
    uint32_t getStructTotalSize(uint32_t structSize);
};

#endif /* NEMEIO_FIRMWAREUPDATE_BANK_FIRMWAREENVIRONMENT_HPP_ */
