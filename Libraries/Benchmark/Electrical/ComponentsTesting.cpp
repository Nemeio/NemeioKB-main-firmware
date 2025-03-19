/*
 * ComponentsTesting.cpp
 *
 *  Created on: 27 avr. 2021
 *      Author: thomas
 */

#include "ComponentsTesting.hpp"
#include "BQ24296Charger.h"
#include "BleGetFwVersionCmd.hpp"

namespace benchmark
{
ComponentsTesting::ComponentsTesting(AssemblyHardware& hardwareAssembly,
                                     ITestLog& log,
                                     PackageFileSystem& firmwarePackageFileSystem)
    : TestSuite(log)
    , m_hardwareAssembly(hardwareAssembly)
    , m_firmwarePackageFileSystem(firmwarePackageFileSystem)
{
}

void ComponentsTesting::notification(const BleFwVersionEvent& event)
{
    m_bleVersion = event.version;

    m_bleVersionSignal.set(0);
}

void ComponentsTesting::notification(BleTxRequestTimeout event)
{
    m_bleVersionSignal.set(0);
}

bool ComponentsTesting::run()
{
    bool ret = true;

    ret = ret
          && registerTestFromIndex(
              ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_ITE_COMMUNICATION);
    ret = ret
          && registerTestFromIndex(
              ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_NRF_COMMUNICATION);
    ret = ret
          && registerTestFromIndex(ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_STM32VERSION);
    ret = ret
          && registerTestFromIndex(
              ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_CHECK_BATTERYCHARGER);

    if(ret)
    {
        ret = runTests("Components Testing");
    }

    unregisterAllTests();

    return ret;
}

bool ComponentsTesting::runSingleTest(uint8_t componentsTestingIndex)
{
    bool ret = registerTestFromIndex(static_cast<ComponentsTestingIndexes>(componentsTestingIndex));

    if(ret)
    {
        ret = runTests("Single Component Test");
    }

    unregisterAllTests();

    return ret;
}

bool ComponentsTesting::registerTestFromIndex(ComponentsTestingIndexes componentsTestingIndex)
{
    bool ret = true;

    switch(componentsTestingIndex)
    {
    case ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_NRF_COMMUNICATION:
        registerTest("NRF Communication/Version",
                     std::bind(&ComponentsTesting::NRFCommunication, this, std::placeholders::_1));
        break;
    case ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_ITE_COMMUNICATION:
        registerTest("ITE Communication/Version",
                     std::bind(&ComponentsTesting::ITECommunication, this, std::placeholders::_1));
        break;
    case ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_STM32VERSION:
        registerTest("STM32 Version",
                     std::bind(&ComponentsTesting::STM32Version, this, std::placeholders::_1));
        break;
    case ComponentsTesting::ComponentsTestingIndexes::SINGLE_COMPONENTSTESTING_CHECK_BATTERYCHARGER:
        registerTest("Battery Charger",
                     std::bind(&ComponentsTesting::checkBatteryCharger,
                               this,
                               std::placeholders::_1));
        break;
    default:
        ret = false;
    }

    return ret;
}


bool ComponentsTesting::ITECommunication(embedded_ostream& description)
{
    bool ret = true;
    char expectedIteVersion[VERSION_MAX_SZ] = {0};
    char lutVersion[VERSION_MAX_SZ] = {0};
    char iteVersion[VERSION_MAX_SZ] = {0};

    /* Get ITE information */
#ifndef NO_ITE_UPDATE
    m_hardwareAssembly.getScreenItem()
        .getAssociatedScreenController()
        .getVersions(iteVersion, sizeof(iteVersion), lutVersion, sizeof(lutVersion));

    /* Get expected version */
    packageVersion expectedVersion;
    if(m_firmwarePackageFileSystem.getVersion(FIRMWARE_PACKAGE_ITE_PATH, expectedVersion))
    {
        snprintf(expectedIteVersion,
                 sizeof(expectedIteVersion),
                 "LDLC_v.%d.%d",
                 expectedVersion.major,
                 expectedVersion.minor);
    }
    else
    {
        ret = false;
    }

    if(ret && 0 != memcmp(iteVersion, expectedIteVersion, strlen(expectedIteVersion)))
    {
        ret = false;
    }

    if(!ret)
    {
        description << "ITE version is " << iteVersion;
        description << " (expected version was " << expectedIteVersion << ")";
    }
#endif

    return ret;
}

bool ComponentsTesting::NRFCommunication(embedded_ostream& description)
{
    bool ret = false;
    BleGetFwVersionCmd cmd;

    memset(&m_bleVersion, 0, sizeof(m_bleVersion));

    /* Get BLE information */
    m_bleVersionSignal.clear();
    cmd.add_observer(*this);

    m_hardwareAssembly.getWatchdogItem().refresh();

    /* Get expected version and component version*/
    packageVersion expectedVersion;
    if(m_firmwarePackageFileSystem.getVersion(FIRMWARE_PACKAGE_NRF_PATH, expectedVersion)
       && m_hardwareAssembly.getBleItem().sendRequest(etl::reference_wrapper(cmd))
              == BLEDefs::ErrCode::SUCCESS
       && m_bleVersionSignal.wait(DelayMs::INFINITE_DELAY)
       && expectedVersion.major == m_bleVersion.major
       && expectedVersion.minor == m_bleVersion.minor)
    {
        ret = true;
    }

    if(!ret)
    {
        description << "Ble version is " << m_bleVersion.major << "." << m_bleVersion.minor;
        description << " (expected version was " << expectedVersion.major << "."
                    << expectedVersion.minor << ")";
    }

    return ret;
}

bool ComponentsTesting::STM32Version(embedded_ostream& description)
{
    bool ret = false;

    m_hardwareAssembly.getWatchdogItem().refresh();

    /* Get expected version */
    packageVersion expectedVersion;
    if(m_firmwarePackageFileSystem.getVersion(FIRMWARE_PACKAGE_STM32_PATH, expectedVersion)
       && expectedVersion.major == APP_MAJOR_NUMBER && expectedVersion.minor == APP_BUILD_NUMBER)
    {
        ret = true;
    }

    if(!ret)
    {
        description << "STM32 version is " << APP_MAJOR_NUMBER << "." << APP_BUILD_NUMBER;
        description << " (expected version was " << expectedVersion.major << "."
                    << expectedVersion.minor << ")";
    }

    return ret;
}


bool ComponentsTesting::checkBatteryCharger(embedded_ostream& description)
{
    bool ret = false;
    uint32_t vendorId = m_hardwareAssembly.getBatteryCharger().getVendorId();

    ret = (vendorId == BQ24296Charger::BQ24296_VENDOR_ID);

    if(!ret)
    {
        description << "Charger id is " << vendorId;
    }

    return ret;
}


} /* namespace benchmark */
