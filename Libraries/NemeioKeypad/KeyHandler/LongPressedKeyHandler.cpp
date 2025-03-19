/*
 * LongPressedKeyHandler.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: amongatcheucheu
 */

#include "LongPressedKeyHandler.h"

LongPressedKeyHandler::LongPressedKeyHandler()
{
}

LongPressedKeyHandler::~LongPressedKeyHandler()
{
    // TODO Auto-generated destructor stub
}

void LongPressedKeyHandler::pressedKeyWithConfig(const ButtonID& button,
                                                 const Configuration& /*config*/)
{
    mCurrentButtonPressed = button;
    notify(true);
    if(xTimerStart(mLongPressTimer, 0) != pdPASS)
    {
    }
}

void LongPressedKeyHandler::unpressedKeyWithConfig(const ButtonID& button,
                                                   const Configuration& /*config*/)
{
    xTimerStop(mLongPressTimer, 0);
    notify(false);
}

void LongPressedKeyHandler::terminateUpdateKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

void LongPressedKeyHandler::resetKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

/*
 * Register a new observer to notify on any keyboad long key pressed event
 */
void LongPressedKeyHandler::registerObserver(iNemeioKeypadPressedListener& observer)
{
    mObserverList.push_back(&observer);
}

/*
 * Unregister an observer
 */
void LongPressedKeyHandler::unregisterObserver(iNemeioKeypadPressedListener& observer)
{
    auto ite = std::find(mObserverList.begin(), mObserverList.end(), &observer);
    if(mObserverList.end() != ite)
    {
        mObserverList.erase(ite);
    }
}

void LongPressedKeyHandler::notify(bool state)
{
    if(state)
    {
        for(auto ite = mObserverList.begin(); ite != mObserverList.end(); ++ite)
        {
            (*ite)->pressedKey(mCurrentButtonPressed);
        }
    }
    else
    {
        for(auto ite = mObserverList.begin(); ite != mObserverList.end(); ++ite)
        {
            (*ite)->unpressedKey(mCurrentButtonPressed);
        }
    }
}

void LongPressedKeyHandler::timerCallback(TimerHandle_t xTimer)
{
    notify(true);
}

std::vector<iNemeioKeypadPressedListener*> LongPressedKeyHandler::mObserverList;
TimerHandle_t LongPressedKeyHandler::mLongPressTimer = xTimerCreate("longPressTimer",
                                                                    0.25 * configTICK_RATE_HZ,
                                                                    pdTRUE,
                                                                    (void*) 0,
                                                                    timerCallback);
ButtonID LongPressedKeyHandler::mCurrentButtonPressed = ButtonID(255, 255);
