#pragma once

#include "ISharedBuffer.hpp"

class ISharedBufferContainer
{
public:
    ISharedBufferContainer() = default;
    virtual ~ISharedBufferContainer() = default;

    virtual ISharedBuffer& getSharedBuffer(uint8_t id) = 0;
};