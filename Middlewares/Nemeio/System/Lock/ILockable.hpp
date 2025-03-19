#pragma once

#include "SystemTypes.hpp"

class ILockable
{
public:
    ILockable() = default;
    virtual ~ILockable() = default;

    virtual bool lock(DelayMs timeout = DelayMs(DelayMs::INFINITE_DELAY)) = 0;
    virtual bool unlock() = 0;
};
