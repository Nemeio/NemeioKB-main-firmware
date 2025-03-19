#pragma once

#include "Timer.hpp"
#include "BleTxCmd.hpp"
#include "AutoLock.h"
#include "Observer.hpp"

struct BleRequestTimerEvent
{
    etl::reference_wrapper<BleTxRequest> request;
};

using BleRequestTimerObserver = etl::observer<const BleRequestTimerEvent&>;

class BleRequestTimer : public ITimerListener,
                        public ProtectedObservable<BleRequestTimerObserver, 1>
{
public:
    explicit BleRequestTimer(BleTxRequest& request)
        : m_request(request)
    {
        m_timer.registerSingleListener(*this);
    }
    virtual ~BleRequestTimer() = default;

    void start()
    {
        m_timer.setDelay(m_request.get().getTimeout());
        m_timer.start();
    };

    void stop() { m_timer.stop(); };

    void timerCallback() final { notify_observers(BleRequestTimerEvent{m_request}); };

private:
    Timer m_timer;
    etl::reference_wrapper<BleTxRequest> m_request;
};