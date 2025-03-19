#pragma once

#include "iNemeioKeypadPressedListener.h"
#include "AssemblyHardware.h"
#include "cmsis_os.h"
#include "IGPIOController.hpp"
#include "ButtonID.hpp"
#include "NemeioKeypad.h"
#include "KeypadScanCodeConverter.hpp"
#include "etl/array.h"

class TestBenchKeypad : public iNemeioKeypadPressedListener
{
public:
    static constexpr size_t MAX_PRESSED_KEYS = ButtonID::INDEX_COUNT + 1;

    explicit TestBenchKeypad(AssemblyHardware& hardwareAssembly);
    ~TestBenchKeypad() final = default;

    void init();
    void deinit();

    void pressedKey(const ButtonID& button) final;
    void unpressedKey(const ButtonID& button) final;
    void terminateUpdateKeys() final;

    etl::array_view<const uint8_t> getPressedKeys();

private:
    static constexpr uint32_t KEYPAD_INIT_DELAY_MS{10};
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

    static constexpr uint8_t POWER_OFF_BUTTON_INDEX = ButtonID::INDEX_COUNT;

    KeypadScanCodeConverter m_keypadScanCodeConverter;
    NemeioKeypad m_keypad;
    IGPIOController& m_gpioController;
    etl::vector<uint8_t, MAX_PRESSED_KEYS> m_pressedKeys;
    ISystem& m_system;
    Mutex m_pressedKeysMutex;

    static void keyboardTask(void const* argument);
    void powerOffButtonInterrupt();

    void addKey(uint8_t index);
    void removeKey(uint8_t index);
};