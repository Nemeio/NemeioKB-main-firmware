/*
 * Assembly.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: cgarnier
 */
#include "FileSystemTrimming.hpp"
#include "timing_macros.h"
#include "../AssemblyHAL/AssemblyHAL.h"

#include "IGPIOController.hpp"

AssemblyHAL::AssemblyHAL(AssemblyHardware& hwAssembly, FileSystemAssembly& fsAssembly)
    : m_battery(hwAssembly.getFuelGauge(),
                fsAssembly.getPartitionFileSystem(FileSystemPartitionLabel::MAIN))
    , m_ledInformation(hwAssembly.getCapsLockLed(),
                       hwAssembly.getBatteryLed(),
                       hwAssembly.getBleLed(),
                       mBatteryTimer,
                       mBleTimer,
                       mUpdateTimer)
    , m_keypad(hwAssembly.getWatchdogItem(),
               hwAssembly.getHardwareTimer(),
               m_keypadScanCodeConverter)

{
}

void AssemblyHAL::init_Assembly()
{
}

void AssemblyHAL::start_Assembly()
{
}

void AssemblyHAL::initPostFS_Assembly()
{
    m_battery.init();
}

Battery& AssemblyHAL::getBattery()
{
    return m_battery;
}

LedInformation& AssemblyHAL::getLedInformationItem()
{
    return m_ledInformation;
}

NemeioKeypad& AssemblyHAL::getKeypadItem()
{
    return m_keypad;
}

KeypadScanCodeConverter& AssemblyHAL::getKeypadScanCodeConverterItem()
{
    return m_keypadScanCodeConverter;
}