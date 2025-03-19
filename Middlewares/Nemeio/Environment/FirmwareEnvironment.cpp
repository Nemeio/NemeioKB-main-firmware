/*
 * FirmwareUpdateEnvironment.cpp
 *
 *  Created on: 27 mars 2021
 *      Author: thomas
 */

#include "FirmwareEnvironment.hpp"
#include "utils.hpp"
#include <cstring>
#include "etl_string_utils.hpp"
#include "FileSystemVersion.hpp"

FirmwareEnvironment::FirmwareEnvironment(etl::array_view<std::byte> env)
{
    envSettings_t* settings = reinterpret_cast<envSettings_t*>(env.data());

    switch(settings->version)
    {
    case 6:
        ASSERT(getStructTotalSize(sizeof(settings->v6)) <= env.size());
        memcpy(&mEnvSettings, settings, getStructTotalSize(sizeof(settings->v6)));
        break;
    case 5:
        ASSERT(getStructTotalSize(sizeof(settings->v5)) <= env.size());
        memcpy(&mEnvSettings, settings, getStructTotalSize(sizeof(settings->v5)));
        memset(mEnvSettings.latest.devTestBenchId, 0, sizeof(mEnvSettings.latest.devTestBenchId));
        memset(mEnvSettings.latest.devBleMacAddress,
               0,
               sizeof(mEnvSettings.latest.devBleMacAddress));
        memset(mEnvSettings.latest.devSerialNumber, 0, sizeof(mEnvSettings.latest.devSerialNumber));
        mEnvSettings.version = 6;
        break;
    case 4:
        ASSERT(getStructTotalSize(sizeof(settings->v4)) <= env.size());
        memcpy(&mEnvSettings, settings, getStructTotalSize(sizeof(settings->v4)));
        mEnvSettings.latest.fsVersion = 0;
        memset(mEnvSettings.latest.devTestBenchId, 0, sizeof(mEnvSettings.latest.devTestBenchId));
        memset(mEnvSettings.latest.devBleMacAddress,
               0,
               sizeof(mEnvSettings.latest.devBleMacAddress));
        memset(mEnvSettings.latest.devSerialNumber, 0, sizeof(mEnvSettings.latest.devSerialNumber));
        mEnvSettings.version = 6;
        break;
    case 3:
    case 2:
    case 1:
        setDefaultSettings(mEnvSettings);
        mEnvSettings.latest.fsVersion = 0;
        break;
    default:
        setDefaultSettings(mEnvSettings);
        break;
    }
}

FirmwareEnvironment::FirmwareEnvironment()
{
    setDefaultSettings(mEnvSettings);
}

void FirmwareEnvironment::setDefaultSettings(envSettings_t& settings)
{
    memset(&mEnvSettings, 0, sizeof(mEnvSettings));
    settings.version = VERSION;
    settings.latest.updateInProgress = false;
    settings.latest.nextFirmwareUpdateBank = FirmwareUpdateBank::UNKNOWN;
    settings.latest.electricalTestsPassed = false;
    settings.latest.functionalTestsPassed = false;
    settings.latest.fsVersion = FILE_SYSTEM_VERSION;
    // testBenchId, devBleMacAddress, devSerialNumber are already zeroed by first memset
}

uint32_t FirmwareEnvironment::getStructTotalSize(uint32_t structSize)
{
    return structSize + member_size(envSettings_t, version);
}

void FirmwareEnvironment::setPendingUpdate(bool pending)
{
    mEnvSettings.latest.updateInProgress = pending;
}

bool FirmwareEnvironment::isPendingUpdate()
{
    return mEnvSettings.latest.updateInProgress;
}

size_t FirmwareEnvironment::toBytes(etl::array_view<std::byte> env) const
{
    if(env.size() < sizeof(mEnvSettings))
    {
        return 0;
    }

    memcpy(env.data(), &mEnvSettings, sizeof(mEnvSettings));

    return sizeof(mEnvSettings);
}

bool FirmwareEnvironment::hasPassedElectricalTests()
{
    return mEnvSettings.latest.electricalTestsPassed;
}

void FirmwareEnvironment::passedElectricalTests(bool passed)
{
    mEnvSettings.latest.electricalTestsPassed = passed;
}

bool FirmwareEnvironment::hasPassedFunctionalTests()
{
    return mEnvSettings.latest.functionalTestsPassed;
}

void FirmwareEnvironment::passedFunctionalTests(bool passed)
{
    mEnvSettings.latest.functionalTestsPassed = passed;
}

FirmwareUpdateBank FirmwareEnvironment::getNextFirmwareUpdateBank()
{
    if(mEnvSettings.latest.nextFirmwareUpdateBank != FirmwareUpdateBank::BANK_A
       && mEnvSettings.latest.nextFirmwareUpdateBank != FirmwareUpdateBank::BANK_B)
    {
        return FirmwareUpdateBank::UNKNOWN;
    }

    return mEnvSettings.latest.nextFirmwareUpdateBank;
}

void FirmwareEnvironment::setNextFirmwareUpdateBank(FirmwareUpdateBank bank)
{
    mEnvSettings.latest.nextFirmwareUpdateBank = bank;
}

uint8_t FirmwareEnvironment::getFileSystemVersion()
{
    return mEnvSettings.latest.fsVersion;
}

void FirmwareEnvironment::setFileSystemVersion(uint8_t fsVersion)
{
    mEnvSettings.latest.fsVersion = fsVersion;
}

#ifdef DEVICE_PROPERTIES_DEBUG_STORAGE
TestBenchId FirmwareEnvironment::getDevTestBenchId() const
{
    return TestBenchId(etl_string_utils::arrayToStringView(
        {mEnvSettings.latest.devTestBenchId,
         mEnvSettings.latest.devTestBenchId + sizeof(mEnvSettings.latest.devTestBenchId)}));
}

void FirmwareEnvironment::setDevTestBenchId(const TestBenchId& testBenchId)
{
    memcpy(mEnvSettings.latest.devTestBenchId, testBenchId.get().data(), testBenchId.get().size());
}

MacAddress FirmwareEnvironment::getDevBleMacAddress() const
{
    etl::array<uint8_t, MacAddress::SIZE> array;
    etl::copy(mEnvSettings.latest.devBleMacAddress,
              mEnvSettings.latest.devBleMacAddress + sizeof(mEnvSettings.latest.devBleMacAddress),
              array.begin());
    return MacAddress(array);
}

void FirmwareEnvironment::setDevBleMacAddress(const MacAddress& macAddress)
{
    memcpy(mEnvSettings.latest.devBleMacAddress, macAddress.get().data(), macAddress.get().size());
}

SerialNumber FirmwareEnvironment::getDevSerialNumber() const
{
    return SerialNumber(etl_string_utils::arrayToStringView(
        {mEnvSettings.latest.devSerialNumber,
         mEnvSettings.latest.devSerialNumber + sizeof(mEnvSettings.latest.devSerialNumber)}));
}

void FirmwareEnvironment::setDevSerialNumber(const SerialNumber& serialNumber)
{
    memcpy(mEnvSettings.latest.devSerialNumber,
           serialNumber.getString().data(),
           serialNumber.getString().size());
}
#endif