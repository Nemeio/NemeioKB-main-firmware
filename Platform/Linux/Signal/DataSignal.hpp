#pragma once

#include <vector>
#include <condition_variable>
#include <queue>
#include "SystemTypes.hpp"

using namespace std::chrono_literals;

static constexpr int32_t DEFAULT_SET_WAIT_DELAY_MS = 10;

template<typename T, size_t SIZE>
class DataSignal
{
public:
    bool waitData(DelayMs delay, T& data);
    void clear();
    void setFromISRData(T& data);
    void setData(const T& data);
    void setData(DelayMs delay, const T& data);

private:
    std::condition_variable m_dataAvailableSignal;
    std::condition_variable m_slotAvailableSignal;
    std::mutex m_condMutex;
    std::mutex m_queueMutex;
    std::queue<T> m_queue;
    size_t count{0};
};


template<typename T, size_t SIZE>
bool DataSignal<T, SIZE>::waitData(DelayMs delay, T& data)
{
    std::unique_lock<std::mutex> lock(m_condMutex);

    if(!m_dataAvailableSignal.wait_for(lock,
                                       delay.getMs() * 1ms,
                                       [this] { return !m_queue.empty(); }))
    {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        data = m_queue.front();
        m_queue.pop();
        --count;
    }

    m_slotAvailableSignal.notify_one();

    return true;
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::setFromISRData(T& data)
{
    setData(0, data);
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::setData(DelayMs delay, const T& data)
{
    std::unique_lock<std::mutex> lock(m_condMutex);

    if(!m_slotAvailableSignal.wait_for(lock, delay.getMs() * 1ms, [this] { return count != SIZE; }))
    {
        return;
    }

    {
        std::lock_guard lock(m_queueMutex);
        m_queue.push(data);
        ++count;
    }

    m_dataAvailableSignal.notify_one();
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::setData(const T& data)
{
    return setData(DelayMs(DEFAULT_SET_WAIT_DELAY_MS), data);
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::clear()
{
    std::lock_guard lock(m_queueMutex);
    std::queue<T> empty;
    std::swap(m_queue, empty);
    count = 0;

    m_slotAvailableSignal.notify_one();
}