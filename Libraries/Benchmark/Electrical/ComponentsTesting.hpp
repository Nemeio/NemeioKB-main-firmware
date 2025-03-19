/*
 * ComponentsTesting.hpp
 *
 *  Created on: 27 avr. 2021
 *      Author: thomas
 */

#ifndef BENCHMARK_ELECTRICAL_COMPONENTSTESTING_HPP_
#define BENCHMARK_ELECTRICAL_COMPONENTSTESTING_HPP_

#include <array>
#include "TestSuite.hpp"
#include "AssemblyHardware.h"
#include "ITestLog.hpp"
#include "Signal.hpp"
#include "Version.h"
#include "PackageFileSystem.hpp"
#include "BleFwVersionObserver.hpp"

namespace benchmark
{
class ComponentsTesting : public TestSuite, public BleFwVersionObserver
{
public:
    ComponentsTesting(AssemblyHardware& hardwareAssembly,
                      ITestLog& log,
                      PackageFileSystem& firmwarePackageFileSystem);
    virtual ~ComponentsTesting() = default;

    bool run() final;

    enum class ComponentsTestingIndexes : uint8_t
    {
        SINGLE_COMPONENTSTESTING_ITE_COMMUNICATION,
        SINGLE_COMPONENTSTESTING_NRF_COMMUNICATION,
        SINGLE_COMPONENTSTESTING_STM32VERSION,
        SINGLE_COMPONENTSTESTING_CHECK_BATTERYCHARGER,
    };

    bool runSingleTest(uint8_t componentsTestingIndex);

    void notification(const BleFwVersionEvent& event) final;
    void notification(BleTxRequestTimeout event) final;

private:
    AssemblyHardware& m_hardwareAssembly;
    PackageFileSystem& m_firmwarePackageFileSystem;
    Signal m_bleVersionSignal;
    BLEDefs::FirmwareVersion m_bleVersion;

    bool registerTestFromIndex(ComponentsTestingIndexes componentsTestingIndex);

    bool ITECommunication(embedded_ostream& description);
    bool NRFCommunication(embedded_ostream& description);
    bool STM32Version(embedded_ostream& description);
    bool checkBatteryCharger(embedded_ostream& description);
};

} /* namespace benchmark */

#endif /* BENCHMARK_ELECTRICAL_COMPONENTSTESTING_HPP_ */
