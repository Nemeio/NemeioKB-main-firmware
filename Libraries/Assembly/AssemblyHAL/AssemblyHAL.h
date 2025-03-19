/*
 * Assembly.h
 *
 *  Created on: Oct 20, 2017
 *      Author: cgarnier
 */

#ifndef ASSEMBLY_ASSEMBLYHAL_ASSEMBLYHAL_H_
#define ASSEMBLY_ASSEMBLYHAL_ASSEMBLYHAL_H_

#include "AssemblyHAL.h"

#include "IAssembly.h"
#include "AssemblyHardware.h"
#include "NullChipSelector.h"
#include "FileSystemTrimming.hpp"

#include "Battery.h"
#include "iNemeioKeypadPressedListener.h"
#include "Timer.hpp"
#include "FileSystemAssembly.hpp"
#include "AssemblyHardware.h"
#include "KeypadScanCodeConverter.hpp"

class UsbHID;

class AssemblyHAL : public IAssembly
{
public:
    explicit AssemblyHAL(AssemblyHardware& hwAssembly, FileSystemAssembly& fsAssembly);
    virtual ~AssemblyHAL() = default;

    void init_Assembly();
    void start_Assembly() final;
    void initPostFS_Assembly();

    [[nodiscard]] NemeioKeypad& getKeypadItem();
    [[nodiscard]] KeypadScanCodeConverter& getKeypadScanCodeConverterItem();

    void registerKeypadListener(iNemeioKeypadPressedListener& ikeypadListener);

    [[nodiscard]] Battery& getBattery();

    [[nodiscard]] LedInformation& getLedInformationItem();

private:
    Battery m_battery;

    Timer mBatteryTimer;
    Timer mBleTimer;
    Timer mUpdateTimer;
    LedInformation m_ledInformation;
    KeypadScanCodeConverter m_keypadScanCodeConverter;
    NemeioKeypad m_keypad;
};

#endif /* ASSEMBLY_ASSEMBLYHAL_ASSEMBLYHAL_H_ */
