/*
 * AssemblyHardware.cpp
 *
 *  Created on: Oct 20, 2017
 *      Author: cgarnier
 */

#include "SFlashConfigurationCommonInterface.hpp"
#include "AssemblyHardware.h"

AssemblyHardware::AssemblyHardware()
    : m_stm32GPIOController()
    , m_stm32ScreenSPI(SCREEN_SPI_INSTANCE)
    , m_IT8951IO(HOLITECH_SCREEN_BUSYN_PIN, HOLITECH_SCREEN_EPD_ON_PIN, HOLITECH_SCREEN_RESN_PIN)
    , m_screenChipSelector(m_stm32GPIOController, HOLITECH_SCREEN_CSN_PIN)
    , m_stm32Watchdog(WATCHDOG_TIMEOUT_SECONDS)
    , mScreenController(m_stm32ScreenSPI,
                        m_screenChipSelector,
                        m_stm32GPIOController,
                        m_IT8951IO,
                        m_stm32Watchdog)
    , m_stm32BLESPI(BLE_SPI_INSTANCE)
    , m_bleIO(BLE_csn_Pin, Ble_Hrdy, Ble_Dts, Ble_Miso, Ble_Mosi)
    , m_bleCommunication(m_stm32BLESPI, m_stm32GPIOController, m_bleIO, m_workQueueNormalPrio)
    , m_bleChip(m_bleCommunication)
    , m_bleHid(m_bleChip)
    , m_usbHID()
    , m_keypadScanTimerAssembly(KEYPAD_SCAN_TIMER_TIM_INSTANCE)
    , m_stm32LedControllerI2C(IS31FL3236A_I2C_INSTANCE)
    , m_stm32FrontlightPWM(FRONTLIGHT_PWM_TIM_INSTANCE, FRONTLIGHT_PWM_TIM_CHANNEL)
    , m_ledController(m_stm32FrontlightPWM)
    , m_batteryCharger(m_stm32LedControllerI2C)
    , m_fuelGauge(m_stm32LedControllerI2C, m_stm32System)
    , m_stm32CRC32()
    , m_capsLockLED(m_stm32GPIOController, Led_CapsLock)
    , m_batteryLED(m_stm32GPIOController, Led_Battery)
    , m_bleLED(m_stm32GPIOController, Led_Ble)
    , mQuadSpi(QUADSPI)
    , mQspiFlashInterface(mQuadSpi)
    , mSpiFlashInterface(m_stm32ScreenSPI, m_screenChipSelector, m_stm32System)
    , mFlashFactory(m_stm32System)
    , mIteFlash(mDummyFlash)
    , mQspiFlash(mDummyFlash)
    , mQspiSecureFlash(mDummyFlash)
{
}

AssemblyHardware::~AssemblyHardware()
{
}

void AssemblyHardware::init_Assembly()
{
    I2CConfig conf = {0};
    conf.timing = 0x10909CEC;
    conf.addressSize = II2C_ADDRESSINGMODE_7BIT;
    conf.dualAdress = II2C_DUALADDRESS_DISABLE;
    conf.ownAddresse2Masks = II2C_OA2_NOMASK;
    conf.generalCallMode = II2C_GENERALCALL_DISABLE;
    conf.noStretchMode = II2C_NOSTRETCH_DISABLE;

    m_stm32Watchdog.init();

    m_stm32LedControllerI2C.init(conf);

    m_fuelGauge.init();

    m_bleChip.init();
    m_bleHid.init();

    SFlashConfigurationCommonInterface flashConfiguration(mQspiFlashInterface);
    mQspiFlashInterface.init(QSPISFlashCommunicationInterface::DEFAULT_FLASH_SIZE);

    uint32_t flashId = flashConfiguration.getIdentifier();
    FlashPair pair = mFlashFactory.makeQspiFlash(flashId, mQspiFlashInterface);
    if(pair.first.has_value())
    {
        mQspiFlash = pair.first.value();
        mQspiFlash.get().init();
    }

    if(pair.second.has_value())
    {
        mQspiSecureFlash = pair.second.value();
        /* Do not initialize since it's already done for the 'main' flash */
    }

    /* Force ID since flash is used by ITE at startup, so we can't communicate with it */
    pair = mFlashFactory.makeSpiFlash(FlashFactory::FLASH_EXTERNAL_MACRONIX_4MB_MANUFACTURER_INFO,
                                      mSpiFlashInterface);
    if(pair.first.has_value())
    {
        mIteFlash = pair.first.value();
        /* Do not initialize since it's used by ITE */
    }

    m_ledController.init();
    m_batteryCharger.init();
    m_stm32CRC32.configure();

    m_capsLockLED.init({IGPIO_OUTTYPE_PUSHPULL, IGPIO_NO_PULL});
    m_batteryLED.init({IGPIO_OUTTYPE_OPENDRAIN, IGPIO_NO_PULL});

    /* Refresh watchdog during screen initialization : If there are timeouts, that might leads to a reset */
    m_stm32Watchdog.refresh();
    mScreenController.init();
    m_stm32Watchdog.refresh();
}

void AssemblyHardware::start_Assembly()
{
}

void AssemblyHardware::initITEFlash()
{
    /* ITE Flash and ITE can't work at the same time */
    mScreenController.holdReset();
    m_stm32ScreenSPI.deinit();
    m_screenChipSelector.deactivate();
    m_stm32ScreenSPI.init(kIteFlashSpiConfig);
    getITEFlash().init();
    getSystemItem().delay(ITE_FLASH_STARTUP_DELAY_MS);
}

void AssemblyHardware::deinitITEFlash()
{
    /* ITE Flash and ITE can't work at the same time */
    getITEFlash().deinit();
    m_screenChipSelector.deactivate();
    m_stm32ScreenSPI.deinit();
    mScreenController.init();
}


IWorkQueue& AssemblyHardware::getWorkQueueNormalPrio()
{
    return m_workQueueNormalPrio;
}

ISystem& AssemblyHardware::getSystemItem()
{
    return m_stm32System;
}

PowerController& AssemblyHardware::getPowerController()
{
    return m_powerController;
}


iSPI& AssemblyHardware::getScreenSPI()
{
    return m_stm32ScreenSPI;
}

IGPIOController& AssemblyHardware::getStm32GPIOController()
{
    return m_stm32GPIOController;
}

BleChip& AssemblyHardware::getBleItem()
{
    return m_bleChip;
}

BleHid& AssemblyHardware::getBleHidItem()
{
    return m_bleHid;
}

UsbHID& AssemblyHardware::getUsbItem()
{
    return m_usbHID;
}

IScreen& AssemblyHardware::getScreenItem()
{
    std::reference_wrapper<IScreen> screen = mDummyScreen;
    if(mScreenController.getScreen().has_value())
    {
        screen = mScreenController.getScreen().value().get();
    }

    return screen;
}

ILedController& AssemblyHardware::getLedController()
{
    return m_ledController;
}

IBatteryCharger& AssemblyHardware::getBatteryCharger()
{
    return m_batteryCharger;
}

IFuelGauge& AssemblyHardware::getFuelGauge()
{
    return m_fuelGauge;
}

ICRC32& AssemblyHardware::getCRC32()
{
    return m_stm32CRC32;
}

AbstractWatchdog& AssemblyHardware::getWatchdogItem()
{
    return m_stm32Watchdog;
}

ILED& AssemblyHardware::getCapsLockLed()
{
    return m_capsLockLED;
}

ILED& AssemblyHardware::getBatteryLed()
{
    return m_batteryLED;
}

ILED& AssemblyHardware::getBleLed()
{
    return m_bleLED;
}

AbstractHWTimer& AssemblyHardware::getHardwareTimer()
{
    return m_keypadScanTimerAssembly.getTimerItem();
}

IFlash& AssemblyHardware::getITEFlash()
{
    return mIteFlash;
}

IFlash& AssemblyHardware::getExternalFlash()
{
    return mQspiFlash;
}

IFlashIO& AssemblyHardware::getExternalSecureFlashIO()
{
    return mQspiSecureFlash;
}
