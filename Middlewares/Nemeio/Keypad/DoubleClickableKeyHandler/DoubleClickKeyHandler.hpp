#pragma once

#include "IKeyMessageHandler.hpp"
#include "IDoubleClickKeyListener.hpp"
#include "etl/vector.h"
#include "ITimer.hpp"
#include "AbstractMutex.hpp"
#include "IWorkQueue.hpp"

class DoubleClickKeyHandler : public IKeyMessageHandler, public ITimerListener
{
public:
    DoubleClickKeyHandler(ITimer& doubleClickDetectTimer,
                          AbstractMutex& mutex,
                          IWorkQueue& workQueue);
    virtual ~DoubleClickKeyHandler() = default;

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

    void timerCallback() final;

    void registerObserver(IDoubleClickKeyListener& observer);
    void unregisterObserver(IDoubleClickKeyListener& observer);

private:
    static constexpr int32_t DOUBLE_CLICK_DELAY_MS = 500;
    static constexpr size_t MAX_OBSERVERS = 3;

    enum class State
    {
        INIT,
        FIRST_CLICK_PRESSED,
        FIRST_CLICK_RELEASED,
    };

    ButtonID m_currentButtonPressed{0, 0};
    State m_state{State::INIT};
    etl::vector<IDoubleClickKeyListener*, MAX_OBSERVERS> m_observerList;
    ITimer& m_doubleClickDetectTimer;
    AbstractMutex& m_mutex;
    IWorkQueue& m_workQueue;
    Work m_timeoutWork;
    Work m_simpleClickWork;
    Work m_doubleClickWork;

    void notifySimpleClick();
    void notifyDoubleClick();

    void onTimeout(etl::array_view<uint8_t> data);
    void onSimpleClickDetected(etl::array_view<uint8_t> data) const;
    void onDoubleClickDetected(etl::array_view<uint8_t> data) const;
};
