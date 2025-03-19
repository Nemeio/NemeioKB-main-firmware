#include "RectangleDataReader.hpp"
#include "assertTools.h"

using namespace benchmark;

RectangleDataReader::RectangleDataReader(size_t width, size_t height, Color color)
    : m_width(width)
    , m_height(height)
    , m_color(color)
{
    ASSERT(width <= (m_buffer.size() * PIXEL_PER_BYTES));
    ASSERT((width % PIXEL_PER_BYTES) == 0);
}

const uint8_t* RectangleDataReader::read(size_t size, size_t& retSize)
{
    ASSERT(size <= m_buffer.size());

    m_buffer.fill(static_cast<uint8_t>(m_color));

    retSize = size;

    return m_buffer.data();
}

bool RectangleDataReader::getSize(std::size_t& size)
{
    size = (m_width * m_height) / PIXEL_PER_BYTES;

    return true;
}

std::size_t RectangleDataReader::getMaxReadSize()
{
    return sizeof(m_buffer);
}
