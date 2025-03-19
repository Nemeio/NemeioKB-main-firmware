#pragma once

#include "embedded_string_stream.hpp"

namespace embedded_io
{
static constexpr size_t DEFAULT_STREAM_SIZE_BYTES = 150;
template<size_t N>
class embedded_ostream_base
{
public:
    embedded_ostream_base()
        : m_stream(m_str)
    {
    }
    template<typename T>
    embedded_ostream_base& operator<<(const T& t)
    {
        m_stream << t;
        return *this;
    }

    auto& str() { return m_str; }
    void clear() { return m_str.clear(); }

private:
    etl::string<N> m_str;
    embedded_string_stream m_stream;
};
} // namespace embedded_io

using embedded_ostream = embedded_io::embedded_ostream_base<embedded_io::DEFAULT_STREAM_SIZE_BYTES>;