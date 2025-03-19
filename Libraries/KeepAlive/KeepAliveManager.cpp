/*
 * KeepAliveManager.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: Labo
 */

#include "KeepAliveManager.hpp"
#include <algorithm>
#include "assertTools.h"

static const uint32_t KEEP_ALIVE_TIMEOUT = 2000;

KeepAliveManager::KeepAliveManager(IWorkQueue& workQueue)
    : mWorkQueue(workQueue)
    , mTimeoutWork([this](etl::array_view<uint8_t> data) { timeoutWork(data); })
{
    mKeepAliveTimer = xTimerCreate("keepAliveTimer",
                                   KEEP_ALIVE_TIMEOUT,
                                   pdFALSE,
                                   this,
                                   keepAliveTimerCallback);
}

KeepAliveManager::~KeepAliveManager()
{
}

void KeepAliveManager::receivedKeepAlive()
{
    resetKeepAliveTimer();

    for(std::vector<IKeepAliveListener*>::iterator ite = mObserverList.begin();
        ite != mObserverList.end();
        ++ite)
    {
        if(*ite != NULL)
        {
            (*ite)->keepAlive();
        }
    }
}

void KeepAliveManager::registerObserver(IKeepAliveListener& observer)
{
    std::vector<IKeepAliveListener*>::iterator ite = std::find(mObserverList.begin(),
                                                               mObserverList.end(),
                                                               &observer);
    if(mObserverList.end() == ite)
    {
        mObserverList.push_back(&observer);
    }
}

void KeepAliveManager::unregisterObserver(IKeepAliveListener& observer)
{
    std::vector<IKeepAliveListener*>::iterator ite = std::find(mObserverList.begin(),
                                                               mObserverList.end(),
                                                               &observer);
    if(mObserverList.end() != ite)
    {
        mObserverList.erase(ite);
    }
}

void KeepAliveManager::keepAliveTimerCallback(TimerHandle_t xTimer)
{
    KeepAliveManager* keepAliveMan = (KeepAliveManager*) pvTimerGetTimerID(xTimer);

    keepAliveMan->timeoutDetected();
}

void KeepAliveManager::resetKeepAliveTimer()
{
    xTimerStop(this->mKeepAliveTimer, 0);
    xTimerStart(this->mKeepAliveTimer, 0);
}

void KeepAliveManager::notifyKeepAliveTimeout()
{
    for(std::vector<IKeepAliveListener*>::iterator ite = mObserverList.begin();
        ite != mObserverList.end();
        ++ite)
    {
        if(*ite != NULL)
        {
            (*ite)->keepAliveTimeout();
        }
    }
}

void KeepAliveManager::timeoutDetected()
{
    ASSERT(mWorkQueue.submit(mTimeoutWork));
}

void KeepAliveManager::timeoutWork(etl::array_view<uint8_t>)
{
    notifyKeepAliveTimeout();
}