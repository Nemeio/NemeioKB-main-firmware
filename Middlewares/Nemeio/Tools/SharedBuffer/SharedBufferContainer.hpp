#pragma once

#include "ISharedBufferContainer.hpp"
#include "SharedBuffer.hpp"
#include "etl/functional.h"

class SharedBufferContainer : public ISharedBufferContainer
{
public:
    SharedBufferContainer() = default;
    ~SharedBufferContainer() final = default;

    ISharedBuffer& getSharedBuffer(uint8_t id) final;

private:
    SharedBuffer<1024> m_buffer1;
    SharedBuffer<4096> m_buffer2;

    etl::array<etl::reference_wrapper<ISharedBuffer>, 2>
        m_buffers{etl::reference_wrapper<ISharedBuffer>{m_buffer1},
                  etl::reference_wrapper<ISharedBuffer>{m_buffer2}};
};