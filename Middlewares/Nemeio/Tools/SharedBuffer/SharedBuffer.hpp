#pragma once

#include "ISharedBuffer.hpp"

template<size_t SIZE>
class SharedBuffer : public ISharedBuffer
{
public:
    SharedBuffer() = default;
    ~SharedBuffer() final = default;

    bool lock(DelayMs timeout) final;
    bool unlock() final;

    etl::array_view<std::byte> buffer() final { return m_buffer; }

private:
    Mutex m_mutex;
    etl::array<std::byte, SIZE> m_buffer;
};

template<size_t SIZE>
bool SharedBuffer<SIZE>::lock(DelayMs timeout)
{
    return m_mutex.lock(timeout);
}

template<size_t SIZE>
bool SharedBuffer<SIZE>::unlock()
{
    return m_mutex.unlock();
}