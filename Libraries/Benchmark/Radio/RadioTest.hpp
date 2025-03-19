/*
 * RadioTest.hpp
 *
 *  Created on: 1 déc. 2021
 *      Author: eskoric
 */

#ifndef BENCHMARK_RADIO_RADIOTEST_HPP_
#define BENCHMARK_RADIO_RADIOTEST_HPP_

#include "IRadioTest.hpp"
#include "BleChip.hpp"
#include "cmsis_os.h"
#include "SystemTypes.hpp"
#include "BleHid.hpp"
#include "BleNumericComparisonCmd.hpp"
#include "Signal.hpp"
#include "cpp_helper.hpp"

class RadioTest : public IRadioTest, public BleNumericComparisonCmdObserver, public BleEventObserver
{
public:
    RadioTest(BleChip& bleChip, BleHid& bleHid);
    virtual ~RadioTest();
    COPYABLE_MOVABLE(RadioTest, delete);

    bool runMode(BLEDefs::TestMode mode) override;

    void notification(const BleNumericComparisonEvent& event) final;

private:
    static constexpr osPriority RADIO_TEST_THREAD_PRIORITY = osPriorityHigh;
    static constexpr size_t RADIO_TEST_THREAD_STACK_SIZE = 512;
    static constexpr size_t RADIO_TEST_QUEUE_NB_ITEMS = 2;
    static constexpr DelayMs RADIO_TEST_QUEUE_SEND_TIMEOUT_DELAY = 10;
    static constexpr DelayMs RADIO_TEST_KEYBOARD_SIMULATION_PERIOD_DELAY = 10;
    static constexpr uint16_t SIMULATE_KEY_ID = 32; // 'E' key (for AZERTY)

    enum class MessageId
    {
        DISABLE_BLE,
        START_KEYBOARD_SIMULATION,
        STOP_KEYBOARD_SIMULATION,
        SET_RADIO_MODE
    };

    struct RadioModeMessage
    {
        BLEDefs::TestMode mode;
    };

    struct Message
    {
        MessageId id;
        union
        {
            RadioModeMessage radioMode;
        };
    };

    static void sRadioTestTask(const void* argument);
    void radioTestTask();

    void startKeyboardSimulation();
    void stopKeyboardSimulation();

    void initKeyboardSimulation();
    void endKeyboardSimulation();
    void setRadioMode(BLEDefs::TestMode mode);
    void disableBle();

    void sendRadioMode(BLEDefs::TestMode mode);

    void simulateKeyPress();

    void turnOffBle();
    void turnOnBle();

    void notification(BleEvent event) final;

    StaticTask_t m_radioTestTask = {0};
    std::array<StackType_t, RADIO_TEST_THREAD_STACK_SIZE> m_radioTestThreadStack = {0};
    osThreadStaticDef(m_radioTestThreadDef,
                      sRadioTestTask,
                      RADIO_TEST_THREAD_PRIORITY,
                      0,
                      RADIO_TEST_THREAD_STACK_SIZE,
                      m_radioTestThreadStack.data(),
                      &m_radioTestTask);

    std::array<uint8_t, RADIO_TEST_QUEUE_NB_ITEMS * sizeof(Message)> m_queueStorageArea;
    QueueHandle_t m_queue;
    StaticQueue_t m_staticQueue;

    BleChip& m_bleChip;
    DelayMs m_queueDelay = DelayMs(DelayMs::INFINITE_DELAY);
    BleHid& m_bleHid;

    Signal m_bleStartedSignal;

    BleNumericComparisonCmd m_numericComparisonCmd;

    bool m_isBleOn{true};
};

#endif /* BENCHMARK_RADIO_RADIOTEST_HPP_ */
