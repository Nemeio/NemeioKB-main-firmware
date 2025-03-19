#pragma once

#include <cstdint>
#include "IDataReader.hpp"
#include "etl/array.h"

namespace benchmark
{
enum class Color : uint8_t
{
    BLACK = 0x0,
    WHITE = 0xFF,
};

class RectangleDataReader : public IDataReader
{
public:
    RectangleDataReader(size_t width, size_t height, Color color);
    ~RectangleDataReader() final = default;

    const uint8_t* read(size_t size, size_t& retSize) final;
    bool getSize(std::size_t& size) final;
    std::size_t getMaxReadSize() final;

private:
    static constexpr size_t RECTANGLE_MAX_WIDTH_PIXELS = 2400;
    static constexpr size_t PIXEL_PER_BYTES = 8;
    static_assert(RECTANGLE_MAX_WIDTH_PIXELS % PIXEL_PER_BYTES == 0);
    etl::array<uint8_t, RECTANGLE_MAX_WIDTH_PIXELS / PIXEL_PER_BYTES> m_buffer;
    size_t m_width;
    size_t m_height;

    Color m_color;
};

} /* namespace benchmark */
