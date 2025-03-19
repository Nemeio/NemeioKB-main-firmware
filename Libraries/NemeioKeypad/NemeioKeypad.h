/*
 * NemeioKeypad.h
 *
 *  Created on: Feb 5, 2018
 *      Author: hector
 */

#ifndef NEMEIOKEYPAD_NEMEIOKEYPAD_H_
#define NEMEIOKEYPAD_NEMEIOKEYPAD_H_

#include <algorithm>
#include "STM32GPIOController.hpp"
#include "Signal.hpp"
#include "AbstractWatchdog.hpp"
#include "KeypadDefs.hpp"
#include "NemeioHWTimerWaitHelper.hpp"
#include "SystemTypes.hpp"
#include "stm32l496xx.h"
#include "IKeypadScanCodeConverter.hpp"
#include "etl/vector.h"

class NemeioKeypad
{
public:
    NemeioKeypad(AbstractWatchdog& watchdog,
                 AbstractHWTimer& hwTimer,
                 IKeypadScanCodeConverter& scanCodeConverter);
    virtual ~NemeioKeypad() = default;
    static void interruptCallback();
    void run();
    void deinitGPIOS();

private:
    static constexpr int32_t DELAY_KEY_PRESSED_MS = 5;
    static constexpr int32_t MARGIN_DELAY_WATCHDOG_REFRESH_SECONDS = 2;
    static constexpr int32_t SCAN_ROW_ACTIVATION_DELAY_US = 1000;
    static constexpr int32_t SCAN_ROW_ACTIVATION_FALLBACK_DELAY_MS = 1;

    static const uint32_t STOP_SCAN_TIMEOUT;
    enum KeypadAction
    {
        KEYPADACTION_NONE,
        KEYPADACTION_KEYPRESSED,
        KEYPADACTION_STOPSCAN
    };

    struct KeypadMessage
    {
        KeypadAction action;

        KeypadMessage()
            : action(KEYPADACTION_NONE)
        {
        }
    };

    STM32GPIOController mGPIO;

    static QueueHandle_t mxQueueKeypad;

    Signal mScanStopSignal;
    AbstractWatchdog& mWatchdog;
    const DelayMs mDelayNoKeyPressed;
    NemeioHWTimerWaitHelper mHWTimerWaitHelper;
    IKeypadScanCodeConverter& mScanCodeConverter;

    void scan(etl::vector<uint16_t, KeypadDefs::MAX_ROW>& scanCodes);
    void initGPIOS();
    void initCallback();
    bool stopScan();
    void rowActivationWait();

    void execStopScan();

    bool execKeyPressed();
};

#endif /* NEMEIOKEYPAD_NEMEIOKEYPAD_H_ */
