#include "TestBenchKeypad.hpp"
#include "AutoLock.h"

TestBenchKeypad::TestBenchKeypad(AssemblyHardware& hardwareAssembly)
    : m_keypad(hardwareAssembly.getWatchdogItem(),
               hardwareAssembly.getHardwareTimer(),
               m_keypadScanCodeConverter)
    , m_gpioController(hardwareAssembly.getStm32GPIOController())
    , m_system(hardwareAssembly.getSystemItem())
{
    m_keypadScanCodeConverter.registerKeyListener(*this);
}

void TestBenchKeypad::init()
{
    std::function<void()> onOffButtonItCallback =
        std::bind(&TestBenchKeypad::powerOffButtonInterrupt, this);

    GPIOInConfig gpioInConfig;
    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_NO_PULL;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING_FALLING;
    gpioInConfig.itCallback = onOffButtonItCallback;

    m_gpioController.configureInput(WKup_3, gpioInConfig);

    osThreadCreate(osThread(mKeyboardThreadDef), this);

    m_system.delay(DelayMs(KEYPAD_INIT_DELAY_MS));
}

void TestBenchKeypad::deinit()
{
    m_keypad.deinitGPIOS();
}

void TestBenchKeypad::keyboardTask(void const* argument)
{
    auto testBenchKeypad = reinterpret_cast<TestBenchKeypad*>(const_cast<void*>(argument));

    testBenchKeypad->m_keypad.run();

    vTaskDelete(nullptr);
}

void TestBenchKeypad::pressedKey(const ButtonID& button)
{
    AutoLock autolock(m_pressedKeysMutex);
    uint8_t index;

    if(button.toIndex(index))
    {
        addKey(index);
    }
}

void TestBenchKeypad::unpressedKey(const ButtonID& button)
{
    AutoLock autolock(m_pressedKeysMutex);
    uint8_t index;

    if(button.toIndex(index))
    {
        removeKey(index);
    }
}

void TestBenchKeypad::terminateUpdateKeys()
{
    // Nothing to do
}

etl::array_view<const uint8_t> TestBenchKeypad::getPressedKeys()
{
    AutoLock autolock(m_pressedKeysMutex);
    return etl::array_view<const uint8_t>(m_pressedKeys.begin(), m_pressedKeys.end());
}

void TestBenchKeypad::powerOffButtonInterrupt()
{
    bool bButtonPressed = (m_gpioController.read(WKup_3) == IGPIO_STATE_LOW);

    if(bButtonPressed)
    {
        addKey(POWER_OFF_BUTTON_INDEX);
    }
    else
    {
        removeKey(POWER_OFF_BUTTON_INDEX);
    }
}

void TestBenchKeypad::addKey(uint8_t index)
{
    m_pressedKeys.push_back(index);
}

void TestBenchKeypad::removeKey(uint8_t index)
{
    auto it = etl::find(m_pressedKeys.begin(), m_pressedKeys.end(), index);

    if(it != m_pressedKeys.end())
    {
        m_pressedKeys.erase(it);
    }
}