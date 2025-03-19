#include "DoubleClickKeyHandler.hpp"
#include "AutoLock.h"

DoubleClickKeyHandler::DoubleClickKeyHandler(ITimer& doubleClickDetectTimer,
                                             AbstractMutex& mutex,
                                             IWorkQueue& workQueue)
    : m_doubleClickDetectTimer(doubleClickDetectTimer)
    , m_mutex(mutex)
    , m_workQueue(workQueue)
    , m_timeoutWork([this](etl::array_view<uint8_t> data) { onTimeout(data); })
    , m_simpleClickWork([this](etl::array_view<uint8_t> data) { onSimpleClickDetected(data); })
    , m_doubleClickWork([this](etl::array_view<uint8_t> data) { onDoubleClickDetected(data); })
{
    m_doubleClickDetectTimer.setDelay(DelayMs(DOUBLE_CLICK_DELAY_MS));
    m_doubleClickDetectTimer.registerSingleListener(*this);
}

void DoubleClickKeyHandler::pressedKeyWithConfig(const ButtonID& button,
                                                 const Configuration& /*config*/)
{
    AutoLock autolock(m_mutex);

    switch(m_state)
    {
    case State::INIT:
        m_currentButtonPressed = button;
        m_doubleClickDetectTimer.start();
        m_state = State::FIRST_CLICK_PRESSED;
        break;
    case State::FIRST_CLICK_RELEASED:
        notifyDoubleClick();
        m_doubleClickDetectTimer.stop();
        m_state = State::INIT;
        break;
    default:
        break;
    }
}

void DoubleClickKeyHandler::unpressedKeyWithConfig(const ButtonID& button,
                                                   const Configuration& /*config*/)
{
    AutoLock autolock(m_mutex);

    switch(m_state)
    {
    case State::FIRST_CLICK_PRESSED:
        m_state = State::FIRST_CLICK_RELEASED;
        break;
    default:
        break;
    }
}

void DoubleClickKeyHandler::terminateUpdateKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

void DoubleClickKeyHandler::resetKeysWithConfig(const Configuration& /*config*/)
{
    // Nothing to do
}

void DoubleClickKeyHandler::onTimeout(etl::array_view<uint8_t>)
{
    AutoLock autolock(m_mutex);

    switch(m_state)
    {
    case State::FIRST_CLICK_PRESSED:
    case State::FIRST_CLICK_RELEASED:
        notifySimpleClick();
        m_state = State::INIT;
        break;
    default:
        break;
    }
}

void DoubleClickKeyHandler::timerCallback()
{
    m_workQueue.submit(m_timeoutWork);
}

void DoubleClickKeyHandler::registerObserver(IDoubleClickKeyListener& observer)
{
    auto ite = std::find(m_observerList.begin(), m_observerList.end(), &observer);
    if(m_observerList.end() == ite)
    {
        m_observerList.push_back(&observer);
    }
}

void DoubleClickKeyHandler::unregisterObserver(IDoubleClickKeyListener& observer)
{
    auto ite = std::find(m_observerList.begin(), m_observerList.end(), &observer);
    if(m_observerList.end() != ite)
    {
        m_observerList.erase(ite);
    }
}

void DoubleClickKeyHandler::notifySimpleClick()
{
    m_workQueue.submit(m_simpleClickWork);
}

void DoubleClickKeyHandler::notifyDoubleClick()
{
    m_workQueue.submit(m_doubleClickWork);
}

void DoubleClickKeyHandler::onSimpleClickDetected(etl::array_view<uint8_t>) const
{
    for(auto& observer: m_observerList)
    {
        if(observer)
        {
            observer->simpleClicked(m_currentButtonPressed);
        }
    }
}

void DoubleClickKeyHandler::onDoubleClickDetected(etl::array_view<uint8_t>) const
{
    for(auto& observer: m_observerList)
    {
        if(observer)
        {
            observer->doubleClicked(m_currentButtonPressed);
        }
    }
}