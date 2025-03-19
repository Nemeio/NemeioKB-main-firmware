/*
 * CemTests.hpp
 *
 *  Created on: 23 nov. 2021
 *      Author: eskoric
 */

#ifndef BENCHMARK_CEMTESTS_HPP_
#define BENCHMARK_CEMTESTS_HPP_

#include "iNemeioKeypadPressedListener.h"
#include "AssemblyHardware.h"
#include "AppCommProxy.hpp"
#include "RadioTestCmd.hpp"
#include "NemeioKeypad.h"
#include "BLEPairing.hpp"
#include "RadioTest.hpp"
#include "NemeioKeypad.h"
#include "KeypadScanCodeConverter.hpp"
#include "cmsis_os.h"

namespace benchmark
{
class CemTests : public iNemeioKeypadPressedListener
{
public:
    explicit CemTests(AssemblyHardware& hardwareAssembly);
    ~CemTests() final = default;

    bool run();

    void pressedKey(const ButtonID& button) final;
    void unpressedKey(const ButtonID& button) final;
    void terminateUpdateKeys() final;

private:
    static constexpr int32_t MARGIN_DELAY_WATCHDOG_REFRESH_SECONDS = 2;
    static constexpr ButtonID DISABLE_BLE_BUTTON{ButtonID(BTN_ESCAPE)};
    static constexpr ButtonID TX_LOWEST_CHANNEL_TEST_BUTTON{ButtonID(BTN_F1)};
    static constexpr ButtonID TX_MIDDLE_CHANNEL_TEST_BUTTON{ButtonID(BTN_F2)};
    static constexpr ButtonID TX_HIGHEST_CHANNEL_TEST_BUTTON{ButtonID(BTN_F3)};
    AssemblyHardware& m_hardwareAssembly;
    BleHid m_bleHid;
    RadioTest m_radioTest;

    static constexpr osPriority KEYBOARD_THREAD_PRIORITY = osPriorityHigh;
    static constexpr size_t KEYBOARD_THREAD_STACK_SIZE = 512;
    StaticTask_t mKeyboardThreadTask = {0};
    StackType_t mKeyboardThreadStack[KEYBOARD_THREAD_STACK_SIZE] = {0};
    osThreadStaticDef(mKeyboardThreadDef,
                      keyboardTask,
                      KEYBOARD_THREAD_PRIORITY,
                      0,
                      KEYBOARD_THREAD_STACK_SIZE,
                      mKeyboardThreadStack,
                      &mKeyboardThreadTask);

    KeypadScanCodeConverter m_keypadScanCodeConverter;
    NemeioKeypad m_keypad;

    static void keyboardTask(void const* argument);
};

} /* namespace benchmark */

#endif /* BENCHMARK_CEMTESTS_HPP_ */
