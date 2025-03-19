#pragma once

#include "etl/array_view.h"
#include "Mutex.hpp"
#include "ILockable.hpp"

class ISharedBuffer : public ILockable
{
public:
    ISharedBuffer() = default;
    ~ISharedBuffer() override = default;

    virtual etl::array_view<std::byte> buffer() = 0;
};
