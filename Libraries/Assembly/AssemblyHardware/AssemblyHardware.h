/*
 * AssemblyHardware.h
 *
 *  Created on: Oct 20, 2017
 *      Author: cgarnier
 */

#ifndef ASSEMBLY_ASSEMBLYHARDWARE_ASSEMBLYHARDWARE_H_
#define ASSEMBLY_ASSEMBLYHARDWARE_ASSEMBLYHARDWARE_H_

#include <IAssembly.h>
#include <STM32QSPI.hpp>
#include <functional>
#include <variant>

#include "STM32GPIOController.hpp"
#include "STM32SPI.h"
#include "UsbHID.h"
#include "STM32I2C.h"
#include "ILedController.hpp"
#include "BQ24296Charger.h"
#include "IBatteryCharger.h"
#include "IFuelGauge.h"
#include "STM32PWM.hpp"
#include "PAM2804/PAM2804Controller.hpp"
#include "../../NemeioKeypad/NemeioKeypad.h"
#include "STM32CRC32.hpp"
#include "../Watchdog/STM32Watchdog.hpp"
#include "../Watchdog/WatchdogConf.hpp"
#include "LED.hpp"
#include "InvertedLED.hpp"
#include "STM32HWTimerAssembly.hpp"
#include "ISystem.hpp"
#include "fs.hpp"
#include "STM32System.hpp"
#include "LedInformation.hpp"
#include "FileSystemAssembly.hpp"
#include "MAX17055.h"
#include "PowerController.hpp"
#include "IT8951Assembly.hpp"
#include "DummyScreen.hpp"
#include "FlashFactory.hpp"
#include "DummyFlash.hpp"
#include "QSPISFlashCommunicationInterface.hpp"
#include "SPISFlashCommunicationInterface.hpp"
#include "NrfCommunication.hpp"
#include "BleChip.hpp"
#include "BleHid.hpp"
#include "WorkQueue.hpp"

#define IS31FL3236A_CONTROLLER_ADDRESS_01 0x78
#define IS31FL3236A_CONTROLLER_ADDRESS_02 0x7E

class AssemblyHardware : public IAssembly
{
public:
    AssemblyHardware();
    virtual ~AssemblyHardware();

    void init_Assembly();
    void start_Assembly();

    void initITEFlash();
    void deinitITEFlash();

    [[nodiscard]] IWorkQueue& getWorkQueueNormalPrio();
    [[nodiscard]] iSPI& getScreenSPI();
    [[nodiscard]] IGPIOController& getStm32GPIOController();
    [[nodiscard]] BleChip& getBleItem();
    [[nodiscard]] BleHid& getBleHidItem();
    [[nodiscard]] UsbHID& getUsbItem();
    [[nodiscard]] IScreen& getScreenItem();

    [[nodiscard]] ILedController& getLedController();

    [[nodiscard]] IBatteryCharger& getBatteryCharger();
    [[nodiscard]] IFuelGauge& getFuelGauge();

    [[nodiscard]] ICRC32& getCRC32();
    [[nodiscard]] AbstractWatchdog& getWatchdogItem();

    [[nodiscard]] ISystem& getSystemItem();
    [[nodiscard]] PowerController& getPowerController();

    [[nodiscard]] ILED& getCapsLockLed();
    [[nodiscard]] ILED& getBatteryLed();
    [[nodiscard]] ILED& getBleLed();

    [[nodiscard]] AbstractHWTimer& getHardwareTimer();

    [[nodiscard]] IFlash& getExternalFlash();
    [[nodiscard]] IFlash& getITEFlash();

    [[nodiscard]] IFlashIO& getExternalSecureFlashIO();

private:
    static constexpr uint32_t ITE_FLASH_STARTUP_DELAY_MS = 10;
    const SPIConfig kIteFlashSpiConfig = {40000000,
                                          ISPI_DATASIZE_8BIT,
                                          ISPI_FIRSTBIT_MSB,
                                          ISPI_POLARITY_LOW,
                                          ISPI_PHASE_1EDGE};

    WorkQueue<1024, 20, osPriorityNormal> m_workQueueNormalPrio;

    STM32System m_stm32System;
    STM32GPIOController m_stm32GPIOController;
    STM32SPI m_stm32ScreenSPI;
    STM32Watchdog m_stm32Watchdog;
    IT8951ScreenController::IT8951IO m_IT8951IO;
    IT8951Assembly mScreenController;
    SingleChipSelector m_screenChipSelector;
    STM32SPI m_stm32BLESPI;
    NrfCommunication::BLEIO m_bleIO;
    NrfCommunication m_bleCommunication;
    BleChip m_bleChip;
    BleHid m_bleHid;
    UsbHID m_usbHID;
    STM32HWTimerAssembly m_keypadScanTimerAssembly;
    STM32I2C m_stm32LedControllerI2C;
    STM32PWM m_stm32FrontlightPWM;
    PAM2804Controller m_ledController;
    BQ24296Charger m_batteryCharger;
    STM32CRC32 m_stm32CRC32;
    LED m_capsLockLED;
    InvertedLED m_batteryLED;
    LED m_bleLED;
    MAX17055 m_fuelGauge;
    PowerController m_powerController;
    DummyScreen mDummyScreen;

    STM32QSPI mQuadSpi;

    /* Flash interface wrapper */
    QSPISFlashCommunicationInterface mQspiFlashInterface;
    SPISFlashCommunicationInterface mSpiFlashInterface;

    FlashFactory mFlashFactory;
    DummyFlash mDummyFlash;

    std::reference_wrapper<IFlash> mIteFlash;
    std::reference_wrapper<IFlash> mQspiFlash;
    std::reference_wrapper<IFlashIO> mQspiSecureFlash;
};

#endif /* ASSEMBLY_ASSEMBLYHARDWARE_ASSEMBLYHARDWARE_H_ */
