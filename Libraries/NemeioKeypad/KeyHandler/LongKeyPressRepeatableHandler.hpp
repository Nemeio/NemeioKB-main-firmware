/*
 * LongKeyPressRepeatableHandler.hpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Labo
 */

#ifndef NEMEIOKEYPAD_LONGPRESSEDKEYHANDLER_LONGKEYPRESSREPEATABLEHANDLER_HPP_
#define NEMEIOKEYPAD_LONGPRESSEDKEYHANDLER_LONGKEYPRESSREPEATABLEHANDLER_HPP_

#include "FreeRTOS.h"
#include "timers.h"
#include <vector>

#include "iNemeioKeypadLongPressedListener.hpp"
#include "IKeyMessageHandler.hpp"

class LongKeyPressRepeatableHandler : public IKeyMessageHandler
{
public:
    LongKeyPressRepeatableHandler();
    virtual ~LongKeyPressRepeatableHandler();

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

    /**
	 * Register a new observer. When a key will be long pressed, observer will be notify with buttonId
	 */
    void registerObserver(iNemeioKeypadLongPressedListener& observer);

    /**
	 * Unregister an observer
	 */
    void unregisterObserver(iNemeioKeypadLongPressedListener& observer);
    void setButtonLongPressDelay(uint16_t buttonLongPressDelay);
    void setButtonRepeatLongPressDelay(uint16_t buttonRepeatLongPressDelay);

private:
    enum LongKeyPressRepeatableState
    {
        KEYSTATE_PRESSED,
        KEYSTATE_UNPRESSED,
        KEYSTATE_LONGPRESSED,
    };

    ButtonID mCurrentButtonPressed;
    LongKeyPressRepeatableState mState;
    uint16_t mButtonLongPressDelay;
    uint16_t mButtonRepeatLongPressDelay;
    TimerHandle_t mLongPressDetectTimer;
    TimerHandle_t mLongPressRepeatTimer;
    std::vector<iNemeioKeypadLongPressedListener*> mObserverList;

    void notifyState();
    void longPressDetected();
    void longPressRepeated();

    static void longPressDetectTimerCallback(TimerHandle_t xTimer);
    static void longPressRepeatTimerCallback(TimerHandle_t xTimer);
};

#endif /* NEMEIOKEYPAD_LONGPRESSEDKEYHANDLER_LONGKEYPRESSREPEATABLEHANDLER_HPP_ */
