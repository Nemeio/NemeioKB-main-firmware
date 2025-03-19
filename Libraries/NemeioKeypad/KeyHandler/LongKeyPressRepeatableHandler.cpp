/*
 * LongKeyPressRepeatableHandler.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Labo
 */

#include "LongKeyPressRepeatableHandler.hpp"

#include <algorithm>

LongKeyPressRepeatableHandler::LongKeyPressRepeatableHandler()
    : mCurrentButtonPressed(255, 255)
    , mState(KEYSTATE_UNPRESSED)
    , mButtonLongPressDelay(1500)
    , mButtonRepeatLongPressDelay(250)
{
    mLongPressDetectTimer = xTimerCreate("longPressDetectTimer",
                                         mButtonLongPressDelay,
                                         pdFALSE,
                                         this,
                                         longPressDetectTimerCallback);
    mLongPressRepeatTimer = xTimerCreate("longPressRepeatTimer",
                                         mButtonRepeatLongPressDelay,
                                         pdTRUE,
                                         this,
                                         longPressRepeatTimerCallback);
}

LongKeyPressRepeatableHandler::~LongKeyPressRepeatableHandler()
{
}

void LongKeyPressRepeatableHandler::pressedKeyWithConfig(const ButtonID& button,
                                                         const Configuration& /*config*/)
{
    mCurrentButtonPressed = button;
    mState = KEYSTATE_PRESSED;
    xTimerChangePeriod(mLongPressDetectTimer, mButtonLongPressDelay, 0);
    xTimerStart(mLongPressDetectTimer, 0);
}

void LongKeyPressRepeatableHandler::unpressedKeyWithConfig(const ButtonID& button,
                                                           const Configuration& /*config*/)
{
    xTimerStop(mLongPressDetectTimer, 0);
    xTimerStop(mLongPressRepeatTimer, 0);
    if(KEYSTATE_PRESSED == mState)
    {
        notifyState();
    }
    mState = KEYSTATE_UNPRESSED;
    notifyState();
}

void LongKeyPressRepeatableHandler::terminateUpdateKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

void LongKeyPressRepeatableHandler::resetKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

void LongKeyPressRepeatableHandler::registerObserver(iNemeioKeypadLongPressedListener& observer)
{
    auto ite = std::find(mObserverList.begin(), mObserverList.end(), &observer);
    if(mObserverList.end() == ite)
    {
        mObserverList.push_back(&observer);
    }
}

void LongKeyPressRepeatableHandler::unregisterObserver(iNemeioKeypadLongPressedListener& observer)
{
    auto ite = std::find(mObserverList.begin(), mObserverList.end(), &observer);
    if(mObserverList.end() != ite)
    {
        mObserverList.erase(ite);
    }
}

void LongKeyPressRepeatableHandler::notifyState()
{
    if(mState == KEYSTATE_PRESSED)
    {
        for(auto ite = mObserverList.begin(); ite != mObserverList.end(); ++ite)
        {
            (*ite)->pressedKey(mCurrentButtonPressed);
        }
    }
    else if(mState == KEYSTATE_UNPRESSED)
    {
        for(auto ite = mObserverList.begin(); ite != mObserverList.end(); ++ite)
        {
            (*ite)->unpressedKey(mCurrentButtonPressed);
        }
    }
    else if(mState == KEYSTATE_LONGPRESSED)
    {
        for(auto ite = mObserverList.begin(); ite != mObserverList.end(); ++ite)
        {
            (*ite)->longPressedKey(mCurrentButtonPressed);
        }
    }
}

void LongKeyPressRepeatableHandler::longPressDetectTimerCallback(TimerHandle_t xTimer)
{
    LongKeyPressRepeatableHandler* handler = (LongKeyPressRepeatableHandler*) pvTimerGetTimerID(
        xTimer);
    if(handler != NULL)
    {
        handler->longPressDetected();
    }
}

void LongKeyPressRepeatableHandler::longPressDetected()
{
    mState = KEYSTATE_LONGPRESSED;
    notifyState();
    xTimerChangePeriod(mLongPressRepeatTimer, mButtonRepeatLongPressDelay, 0);
    xTimerStart(mLongPressRepeatTimer, 0);
}

void LongKeyPressRepeatableHandler::longPressRepeated()
{
    if(KEYSTATE_LONGPRESSED == mState)
    {
        notifyState();
    }
}

void LongKeyPressRepeatableHandler::longPressRepeatTimerCallback(TimerHandle_t xTimer)
{
    LongKeyPressRepeatableHandler* handler = (LongKeyPressRepeatableHandler*) pvTimerGetTimerID(
        xTimer);
    if(handler != NULL)
    {
        handler->longPressRepeated();
    }
}

void LongKeyPressRepeatableHandler::setButtonLongPressDelay(uint16_t buttonLongPressDelay)
{
    mButtonLongPressDelay = buttonLongPressDelay;
}

void LongKeyPressRepeatableHandler::setButtonRepeatLongPressDelay(uint16_t buttonRepeatLongPressDelay)
{
    mButtonRepeatLongPressDelay = buttonRepeatLongPressDelay;
}
