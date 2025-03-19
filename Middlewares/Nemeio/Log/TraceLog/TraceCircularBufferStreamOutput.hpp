#pragma once

#include "circular_ostream.hpp"
#include <cstdio>

namespace logger
{
template<size_t BufferSize>
class TraceCircularBufferStreamOutput : public embedded_io::CircularBufferStreamOutput<BufferSize>
{
public:
    void write(etl::string_view s);
};

template<size_t BufferSize>
void TraceCircularBufferStreamOutput<BufferSize>::write(etl::string_view s)
{
    embedded_io::CircularBufferStreamOutput<BufferSize>::write(s);
    printf("%s", s.data());
};
} // namespace logger