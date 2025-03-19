#pragma once

#include <functional>
#include "etl/array_view.h"

class Work
{
public:
    Work() = default;
    explicit Work(std::function<void(etl::array_view<uint8_t> data)>& func);
    explicit Work(std::function<void(etl::array_view<uint8_t> data)>&& func);
    virtual ~Work() = default;

    void run(etl::array_view<uint8_t> data) const;

private:
    std::function<void(etl::array_view<uint8_t> data)> m_func;
};

inline Work::Work(std::function<void(etl::array_view<uint8_t> data)>& func)
    : m_func(func)
{
}

inline Work::Work(std::function<void(etl::array_view<uint8_t> data)>&& func)
    : m_func(func)
{
}

inline void Work::run(etl::array_view<uint8_t> data) const
{
    if(m_func)
    {
        m_func(data);
    }
}