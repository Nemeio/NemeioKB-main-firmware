/*
 * NemeioKeypad.cpp
 *
 *  Created on: Feb 5, 2018
 *      Author: hector
 */

#include "../NemeioKeypad/NemeioKeypad.h"

#include <vector>
#include "cmsis_os.h"
#include "FreeRTOSUtils.hpp"
#include <new>

/* rowsPins :
 * Used to iterate over all rows to set them at a common state.
 */
static const uint16_t rowsPins[] =
    {KB_LIN_1_Pin, KB_LIN_2_Pin, KB_LIN_3_Pin, KB_LIN_4_Pin, KB_LIN_5_Pin, KB_LIN_6_Pin};

/* columnsPins :
 * Currently unused since all the pins are on the same bank.
 * For performance and coherency reasons it is better to read the 
 * whole bank at once
 */
static const uint16_t columnsPins[] = {
    KB_COL_1_Pin,
    KB_COL_2_Pin,
    KB_COL_3_Pin,
    KB_COL_4_Pin,
    KB_COL_5_Pin,
    KB_COL_6_Pin,
    KB_COL_7_Pin,
    KB_COL_8_Pin,
    KB_COL_9_Pin,
    KB_COL_10_Pin,
    KB_COL_11_Pin,
    KB_COL_12_Pin,
    KB_COL_13_Pin,
    KB_COL_14_Pin,
    KB_COL_15_Pin,
    KB_COL_16_Pin,
};

static const GPIOPin_t independantPins[] = {
    Key75_Alt,
    Key73_Fn,
    Key74_Win,
    Key57_Enter,
    Key58_Shift_Left,
    Key71_Shift_Right,
    Key_Ctrl_Left,
    Key_Ctrl_Right,
    Key_Alt_Gr,
    Btn_Left,
    Btn_Right,
};

#define KEY_DOWN 0
#define KEY_UP 1

struct GPIOHandlePinState_t
{
    GPIOPin gpioPin;
    GPIOPinState gpioPinState;
};

/* Queue used to notify polling thread of a state change in CPU_WAKE_UP */
QueueHandle_t NemeioKeypad::mxQueueKeypad = {0};

const uint32_t NemeioKeypad::STOP_SCAN_TIMEOUT = 5000;

NemeioKeypad::NemeioKeypad(AbstractWatchdog& watchdog,
                           AbstractHWTimer& hwTimer,
                           IKeypadScanCodeConverter& scanCodeConverter)
    : mGPIO()
    , mScanStopSignal()
    , mWatchdog(watchdog)
    , mDelayNoKeyPressed(
          DelaySec(mWatchdog.getTimeoutSeconds() - MARGIN_DELAY_WATCHDOG_REFRESH_SECONDS))
    , mHWTimerWaitHelper(hwTimer)
    , mScanCodeConverter(scanCodeConverter)
{
    this->initGPIOS();
    this->initCallback();

    NemeioKeypad::mxQueueKeypad = xQueueCreate(KeypadDefs::MAX_ROW + 1, sizeof(KeypadMessage));
}

/*
 * Setup all parameters for a callback on CPU_WAKE_UP
 */
void NemeioKeypad::initCallback()
{
    GPIOInConfig gpioInConfig;

    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_NO_PULL;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING_FALLING;
    gpioInConfig.itCallback = interruptCallback;
    mGPIO.configureInput(Cpu_Wake_Up, gpioInConfig);
}

/*
 * Callback called on CPU_WAKE_UP IRQ
 * Pushes the state of CPU_WAKE_UP on a queue then tries to wake up
 * the polling thread that is possibly waiting on the queue
 */
void NemeioKeypad::interruptCallback()
{
    BaseType_t xHigherPriorityTaskWoken;
    KeypadMessage message;
    message.action = KEYPADACTION_KEYPRESSED;

    xQueueSendToBackFromISR(NemeioKeypad::mxQueueKeypad, &message, &xHigherPriorityTaskWoken);

    if(xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/*
 * Init GPIOS for Nemeio keyboard
 */
void NemeioKeypad::initGPIOS()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*
	 * Init Rows GPIOS
	 * Rows are set as output and will be switched on one by one to scan
	 * the active columns
	 */
#ifdef USE_ETM
    GPIO_InitStruct.Pin = KB_LIN_1_Pin | KB_LIN_2_Pin /* | KB_LIN_3_Pin
							| KB_LIN_4_Pin | KB_LIN_5_Pin | KB_LIN_6_Pin*/
        ;
#else
    GPIO_InitStruct.Pin = KB_LIN_1_Pin | KB_LIN_2_Pin | KB_LIN_3_Pin | KB_LIN_4_Pin | KB_LIN_5_Pin
                          | KB_LIN_6_Pin;
#endif

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOE,
                      KB_LIN_1_Pin | KB_LIN_2_Pin | KB_LIN_3_Pin | KB_LIN_4_Pin | KB_LIN_5_Pin
                          | KB_LIN_6_Pin,
                      GPIO_PIN_RESET);

    /*
	 * Init Columns GPIOS
	 * Columns are set as input and will be scanned after each row is activated
	 */
    GPIO_InitStruct.Pin = KB_COL_1_Pin | KB_COL_2_Pin | KB_COL_3_Pin | KB_COL_4_Pin | KB_COL_5_Pin
                          | KB_COL_6_Pin | KB_COL_7_Pin | KB_COL_8_Pin | KB_COL_9_Pin
                          | KB_COL_10_Pin | KB_COL_11_Pin | KB_COL_12_Pin | KB_COL_13_Pin
                          | KB_COL_14_Pin | KB_COL_15_Pin | KB_COL_16_Pin;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KB_KEY75_ALT_Pin | KB_KEY73_FN_Pin | KB_KEY74_WIN_Pin | KB_KEY57_ENTER_Pin
                          | KB_KEY58_SHIFT_LEFT_Pin | KB_KEY71_SHIFT_RIGHT_Pin
                          | KB_KEY72_CTRL_LEFT_Pin | KB_KEY78_CTRL_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KB_KEY77_ALT_GR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Button_LEFT_Pin | Button_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/*
 * Performs a scan of the keyboard :
 * Should only be called with IRQ disabled on CPU_WAKE_UP
 */
void NemeioKeypad::scan(etl::vector<uint16_t, KeypadDefs::MAX_ROW>& scanCodes)
{
    /* Activate Rows one by one */
    for(unsigned int row = 0; row < sizeof(rowsPins) / sizeof(rowsPins[0]); row++)
    {
        HAL_GPIO_WritePin(GPIOE, rowsPins[row], GPIO_PIN_RESET);

        rowActivationWait();

        uint16_t scanCode = GPIOD->IDR & 0xFFFF;
        /* Since all lines are on GPIO BANK D push the whole bank */
        scanCodes.push_back(scanCode);

        HAL_GPIO_WritePin(GPIOE, rowsPins[row], GPIO_PIN_SET);
    }

    uint16_t indPinScanCode = 0xFFFF;

    for(unsigned int idx = 0; idx < sizeof(independantPins) / sizeof(independantPins[0]); idx++)
    {
        if(IGPIO_STATE_LOW == mGPIO.read(independantPins[idx]))
        {
            indPinScanCode &= ~(1 << idx);
        }
    }
    scanCodes.push_back(indPinScanCode);
}

void NemeioKeypad::run()
{
    bool isKeyPressed = execKeyPressed();

    /* Infinite loop to keep main thread alive */
    for(;;)
    {
        KeypadMessage message;

        mWatchdog.refresh();

        /* Wait on the queue that will be populated by IRQ
		 * - wait watchdog timeout when no key is pressed : wake up by IRQ or watchdog timeout
		 * - wait DELAY_KEY_PRESSED ticks when a key is pressed : wake up by timeout
		 */
        xQueueReceive(NemeioKeypad::mxQueueKeypad,
                      &message,
                      delayToSystemTicks(isKeyPressed ? DelayMs(DELAY_KEY_PRESSED_MS)
                                                      : mDelayNoKeyPressed));

        if(message.action == KEYPADACTION_STOPSCAN)
        {
            execStopScan();
            // Exit infinite scan loop
            break;
        }
        else if((message.action == KEYPADACTION_KEYPRESSED)
                || (isKeyPressed && (message.action == KEYPADACTION_NONE)))
        {
            isKeyPressed = execKeyPressed();
        }
    }
}

void NemeioKeypad::deinitGPIOS()
{
    stopScan();

    uint32_t rowPin = KB_LIN_1_Pin | KB_LIN_2_Pin | KB_LIN_3_Pin | KB_LIN_4_Pin | KB_LIN_5_Pin
                      | KB_LIN_6_Pin;
    HAL_GPIO_DeInit(GPIOE, rowPin);

    uint32_t colPin = KB_COL_1_Pin | KB_COL_2_Pin | KB_COL_3_Pin | KB_COL_4_Pin | KB_COL_5_Pin
                      | KB_COL_6_Pin | KB_COL_7_Pin | KB_COL_8_Pin | KB_COL_9_Pin | KB_COL_10_Pin
                      | KB_COL_11_Pin | KB_COL_12_Pin | KB_COL_13_Pin | KB_COL_14_Pin
                      | KB_COL_15_Pin | KB_COL_16_Pin;

    HAL_GPIO_DeInit(GPIOD, colPin);
}

bool NemeioKeypad::stopScan()
{
    bool ret = false;

    KeypadMessage message;
    message.action = KEYPADACTION_STOPSCAN;

    mScanStopSignal.clear();
    if(pdTRUE == xQueueSendToBack(mxQueueKeypad, (void*) &message, (TickType_t) STOP_SCAN_TIMEOUT))
    {
        if(mScanStopSignal.wait(DelayMs(DelayMs::INFINITE_DELAY)))
        {
            ret = true;
        }
    }

    return ret;
}

void NemeioKeypad::execStopScan()
{
    mScanStopSignal.set();
}

void NemeioKeypad::rowActivationWait()
{
    HWTimerWaitHelper::RetCode waitRet = mHWTimerWaitHelper.wait(
        DelayUs(SCAN_ROW_ACTIVATION_DELAY_US));

    ASSERT(waitRet != HWTimerWaitHelper::RetCode::INVALID_PARAMETER);

    if(HWTimerWaitHelper::RetCode::SUCCESS != waitRet
       && HWTimerWaitHelper::RetCode::TIMEOUT != waitRet)
    {
        // If hw timer control has failed and wait process has not been done, use os delay as a fallback
        osDelay(SCAN_ROW_ACTIVATION_FALLBACK_DELAY_MS + 1);
    }
}

bool NemeioKeypad::execKeyPressed()
{
    /*
	 * If we reach here the IRQ populated the queue so we have a key press
	 * Disable IRQ since scanning would trigger more of them
	 * Set all rows to 1
	 */
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

    for(auto rowPin: rowsPins)
    {
        HAL_GPIO_WritePin(GPIOE, rowPin, GPIO_PIN_SET);
    }

    etl::vector<uint16_t, KeypadDefs::MAX_ROW> scanCodes;
    this->scan(scanCodes);

    /* Check if any key on the mapping is pressed */
    bool keyPressed = false;
    for(auto rowScanCodes: scanCodes)
    {
        if(rowScanCodes != 0xFFFF)
        {
            keyPressed = true;
            break;
        }
    }

    /* If not, re enable IRQ and go to sleep on the queue */
    if(!keyPressed)
    {
        for(auto rowPin: rowsPins)
        {
            HAL_GPIO_WritePin(GPIOE, rowPin, GPIO_PIN_RESET);
        }
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    }

    mScanCodeConverter.convertScanCodeToKeyUpdate(scanCodes);

    return keyPressed;
}
