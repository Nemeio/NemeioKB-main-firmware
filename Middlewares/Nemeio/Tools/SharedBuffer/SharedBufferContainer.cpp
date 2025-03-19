#include "SharedBufferContainer.hpp"
#include "assertTools.h"

ISharedBuffer& SharedBufferContainer::getSharedBuffer(uint8_t id)
{
    ASSERT(id < m_buffers.size());

    return m_buffers[id];
}