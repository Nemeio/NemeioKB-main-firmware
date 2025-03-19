/*
 * CemTests.cpp
 *
 *  Created on: 23 nov. 2021
 *      Author: eskoric
 */

#include "CemTests.hpp"

using namespace benchmark;

CemTests::CemTests(AssemblyHardware& hardwareAssembly)
    : m_hardwareAssembly(hardwareAssembly)
    , m_bleHid(m_hardwareAssembly.getBleItem())
    , m_radioTest(m_hardwareAssembly.getBleItem(), m_bleHid)
    , m_keypad(hardwareAssembly.getWatchdogItem(),
               hardwareAssembly.getHardwareTimer(),
               m_keypadScanCodeConverter)
{
    m_keypadScanCodeConverter.registerKeyListener(*this);

    osThreadCreate(osThread(mKeyboardThreadDef), this);
}

bool CemTests::run()
{
    for(;;)
    {
        m_hardwareAssembly.getWatchdogItem().refresh();
        osDelay(delayToMsCMSIS(DelaySec(m_hardwareAssembly.getWatchdogItem().getTimeoutSeconds()
                                        - MARGIN_DELAY_WATCHDOG_REFRESH_SECONDS)));
    }

    return false;
}

void CemTests::pressedKey(const ButtonID& button)
{
    BLEDefs::TestMode mode = BLEDefs::TestMode::UNKNOWN;

    if(button == DISABLE_BLE_BUTTON)
    {
        mode = BLEDefs::TestMode::DISABLED;
    }
    else if(button == TX_LOWEST_CHANNEL_TEST_BUTTON)
    {
        mode = BLEDefs::TestMode::TX_LOWEST_CHANNEL;
    }
    else if(button == TX_MIDDLE_CHANNEL_TEST_BUTTON)
    {
        mode = BLEDefs::TestMode::TX_MIDDLE_CHANNEL;
    }
    else if(button == TX_HIGHEST_CHANNEL_TEST_BUTTON)
    {
        mode = BLEDefs::TestMode::TX_HIGHEST_CHANNEL;
    }

    if(mode != BLEDefs::TestMode::UNKNOWN)
    {
        m_radioTest.runMode(mode);
    }
}

void CemTests::unpressedKey(const ButtonID& button)
{
    // Nothing to do
}

void CemTests::terminateUpdateKeys()
{
    // Nothing to do
}

void CemTests::keyboardTask(void const* argument)
{
    auto cemTests = reinterpret_cast<CemTests*>(const_cast<void*>(argument));

    cemTests->m_keypad.run();

    vTaskDelete(nullptr);
}