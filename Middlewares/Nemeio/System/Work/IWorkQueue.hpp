#pragma once

#include "Work.hpp"

class IWorkQueue
{
public:
    IWorkQueue() = default;
    virtual ~IWorkQueue() = default;

    virtual bool submit(Work& work) = 0;
    virtual bool submitData(Work& work, etl::array_view<const uint8_t> data) = 0;
};