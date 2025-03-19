/*
 * LongPressedKeyHandler.h
 *
 *	Class that handles long key pressed. If an another part of application would like to determine if a key is long pressed,
 *	it should register at this handler.
 *
 *  Created on: Feb 27, 2018
 *      Author: amongatcheucheu
 */

#ifndef NEMEIOKEYPAD_LONGPRESSEDKEYHANDLER_LONGPRESSEDKEYHANDLER_H_
#define NEMEIOKEYPAD_LONGPRESSEDKEYHANDLER_LONGPRESSEDKEYHANDLER_H_

#include "FreeRTOS.h"
#include "timers.h"
#include <vector>
#include <algorithm>

#include "IKeyMessageHandler.hpp"
#include "iNemeioKeypadPressedListener.h"

class LongPressedKeyHandler : public IKeyMessageHandler
{
public:
    LongPressedKeyHandler();
    virtual ~LongPressedKeyHandler();

    /**
	 * Register a new observer. When a key will be long pressed, observer will be notify with buttonId
	 */
    void registerObserver(iNemeioKeypadPressedListener& observer);

    /**
	 * Unregister an observer
	 */
    void unregisterObserver(iNemeioKeypadPressedListener& observer);

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

private:
    static TimerHandle_t mLongPressTimer;
    static std::vector<iNemeioKeypadPressedListener*> mObserverList;
    static ButtonID mCurrentButtonPressed;

    static void notify(bool state);
    static void timerCallback(TimerHandle_t xTimer);
};

#endif /* NEMEIOKEYPAD_LONGPRESSEDKEYHANDLER_LONGPRESSEDKEYHANDLER_H_ */
