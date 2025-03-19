/*
 * RadioTest.cpp
 *
 *  Created on: 1 déc. 2021
 *      Author: eskoric
 */

#include "RadioTest.hpp"
#include "FreeRTOSUtils.hpp"
#include "BleSetAdvertisingCmd.hpp"
#include "BleSetRadioModeCmd.hpp"
#include "BleNumericComparisonCmd.hpp"
#include "TraceLogger.hpp"

#define MODULE "radiotest"

RadioTest::RadioTest(BleChip& bleChip, BleHid& bleHid)
    : m_bleChip(bleChip)
    , m_bleHid(bleHid)
{
    m_queue = xQueueCreateStatic(RADIO_TEST_QUEUE_NB_ITEMS,
                                 sizeof(Message),
                                 m_queueStorageArea.data(),
                                 &m_staticQueue);

    configASSERT(m_queue);

    osThreadCreate(osThread(m_radioTestThreadDef), this);

    m_bleChip.registerBleCommandListener(m_numericComparisonCmd);
    m_bleChip.add_observer(*this);
    m_numericComparisonCmd.add_observer(*this);
}

RadioTest::~RadioTest()
{
    m_bleChip.unregisterBleCommandListener(m_numericComparisonCmd);
    m_bleChip.remove_observer(*this);
}

bool RadioTest::runMode(BLEDefs::TestMode mode)
{
    bool ret = true;

    switch(mode)
    {
    case BLEDefs::TestMode::DISABLED:
        disableBle();
        break;
    case BLEDefs::TestMode::TX_LOWEST_CHANNEL:
    case BLEDefs::TestMode::TX_MIDDLE_CHANNEL:
    case BLEDefs::TestMode::TX_HIGHEST_CHANNEL:
    case BLEDefs::TestMode::RX:
        stopKeyboardSimulation();
        setRadioMode(mode);
        break;
    case BLEDefs::TestMode::APP_HID_DATA:
        startKeyboardSimulation();
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

void RadioTest::sRadioTestTask(const void* argument)
{
    RadioTest* pRadioTest = reinterpret_cast<RadioTest*>(const_cast<void*>(argument));

    pRadioTest->radioTestTask();
}

void RadioTest::radioTestTask()
{
    Message message;

    for(;;)
    {
        if(pdPASS == xQueueReceive(m_queue, &message, delayToSystemTicks(m_queueDelay)))
        {
            switch(message.id)
            {
            case MessageId::DISABLE_BLE:
                turnOffBle();
                break;
            case MessageId::START_KEYBOARD_SIMULATION:
                initKeyboardSimulation();
                break;
            case MessageId::STOP_KEYBOARD_SIMULATION:
                endKeyboardSimulation();
                break;
            case MessageId::SET_RADIO_MODE:
                sendRadioMode(message.radioMode.mode);
                break;
            default:
                break;
            }
        }
        else
        {
            simulateKeyPress();
        }
    }
}

void RadioTest::disableBle()
{
    Message message;
    message.id = MessageId::DISABLE_BLE;

    xQueueSendToBack(m_queue, &message, delayToSystemTicks(RADIO_TEST_QUEUE_SEND_TIMEOUT_DELAY));
}

void RadioTest::startKeyboardSimulation()
{
    Message message;
    message.id = MessageId::START_KEYBOARD_SIMULATION;

    xQueueSendToBack(m_queue, &message, delayToSystemTicks(RADIO_TEST_QUEUE_SEND_TIMEOUT_DELAY));
}

void RadioTest::stopKeyboardSimulation()
{
    Message message;
    message.id = MessageId::STOP_KEYBOARD_SIMULATION;

    xQueueSendToBack(m_queue, &message, delayToSystemTicks(RADIO_TEST_QUEUE_SEND_TIMEOUT_DELAY));
}

void RadioTest::setRadioMode(BLEDefs::TestMode mode)
{
    Message message;
    message.id = MessageId::SET_RADIO_MODE;
    message.radioMode.mode = mode;

    xQueueSendToBack(m_queue, &message, delayToSystemTicks(RADIO_TEST_QUEUE_SEND_TIMEOUT_DELAY));
}

void RadioTest::initKeyboardSimulation()
{
    m_queueDelay = DelayMs(RADIO_TEST_KEYBOARD_SIMULATION_PERIOD_DELAY);

    turnOffBle();
    turnOnBle();
    m_bleChip.configure();

    m_bleChip.enableBle();
}

void RadioTest::endKeyboardSimulation()
{
    m_queueDelay = DelayMs(DelayMs::INFINITE_DELAY);
}

void RadioTest::simulateKeyPress()
{
    m_bleHid.selectTouch(SIMULATE_KEY_ID, true, false);
    m_bleHid.sendReports();
    osDelay(delayToMsCMSIS(RADIO_TEST_KEYBOARD_SIMULATION_PERIOD_DELAY));
    m_bleHid.selectTouch(SIMULATE_KEY_ID, false, false);
    m_bleHid.sendReports();
}

void RadioTest::turnOnBle()
{
    if(!m_isBleOn)
    {
        m_bleChip.init();
        m_bleStartedSignal.wait(DelayMs::INFINITE_DELAY);

        m_isBleOn = true;
    }
}

void RadioTest::turnOffBle()
{
    if(m_isBleOn)
    {
        m_bleChip.powerOff();

        LOG_INFO(MODULE, "disable BLE");

        m_isBleOn = false;
    }
}

void RadioTest::sendRadioMode(BLEDefs::TestMode mode)
{
    BleSetRadioModeCmd cmd;
    cmd.setMode(mode);

    turnOnBle();

    LOG_INFO(MODULE, "send radio mode " << (int) mode);

    m_bleChip.sendCommand(BleApplicationTxCommandRef(etl::reference_wrapper(cmd)));
}

void RadioTest::notification(const BleNumericComparisonEvent& event)
{
    // Accept pairing by default
    m_numericComparisonCmd.setAccept(true);
    m_bleChip.sendCommand(
        BleReadyForBleTxCommandRef(etl::reference_wrapper(m_numericComparisonCmd)));
}

void RadioTest::notification(BleEvent event)
{
    if(event == BleEvent::STARTED)
    {
        m_bleStartedSignal.set();
    }
}