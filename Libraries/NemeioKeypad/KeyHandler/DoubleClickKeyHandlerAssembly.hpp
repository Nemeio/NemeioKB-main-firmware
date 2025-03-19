#pragma once

#include "Mutex.hpp"
#include "Timer.hpp"
#include "DoubleClickKeyHandler.hpp"

class DoubleClickKeyHandlerAssembly
{
public:
    explicit DoubleClickKeyHandlerAssembly(IWorkQueue& workQueue);
    virtual ~DoubleClickKeyHandlerAssembly() = default;

    DoubleClickKeyHandler& getItem();

private:
    Timer m_timer;
    Mutex m_mutex;
    DoubleClickKeyHandler m_keyHandler;
};

inline DoubleClickKeyHandlerAssembly::DoubleClickKeyHandlerAssembly(IWorkQueue& workQueue)
    : m_keyHandler(m_timer, m_mutex, workQueue)
{
}

inline DoubleClickKeyHandler& DoubleClickKeyHandlerAssembly::getItem()
{
    return m_keyHandler;
}