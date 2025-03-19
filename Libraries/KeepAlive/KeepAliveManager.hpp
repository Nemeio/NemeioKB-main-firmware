/*
 * KeepAliveManager.hpp
 *
 *  Created on: Sep 20, 2018
 *      Author: Labo
 */

#ifndef KEEPALIVE_KEEPALIVEMANAGER_HPP_
#define KEEPALIVE_KEEPALIVEMANAGER_HPP_

#include "IKeepAliveManager.hpp"
#include "IKeepAliveListener.hpp"
#include <vector>
#include "FreeRTOS.h"
#include "timers.h"
#include "IWorkQueue.hpp"

class KeepAliveManager : public IKeepAliveManager
{
public:
    KeepAliveManager(IWorkQueue& workQueue);
    virtual ~KeepAliveManager();

    void receivedKeepAlive();

    void registerObserver(IKeepAliveListener& observer);
    void unregisterObserver(IKeepAliveListener& observer);

private:
    static constexpr UBaseType_t KEEP_ALIVE_MANAGER_TASK_PRIORITY = 3;

    std::vector<IKeepAliveListener*> mObserverList;
    TimerHandle_t mKeepAliveTimer;

    IWorkQueue& mWorkQueue;
    Work mTimeoutWork;

    void resetKeepAliveTimer();
    static void keepAliveTimerCallback(TimerHandle_t xTimer);

    void notifyKeepAliveTimeout();
    void timeoutDetected();
    void timeoutWork(etl::array_view<uint8_t> data);
};

#endif /* KEEPALIVE_KEEPALIVEMANAGER_HPP_ */
