#pragma once

#include "embedded_string_stream.hpp"
#include "etl/circular_buffer.h"
#include "embedded_ostream.hpp"
#include "etl/array_view.h"

namespace embedded_io
{
template<size_t BufferSize>
class CircularBufferStreamOutput
{
public:
    void write(etl::string_view s) { m_buffer.push(s.begin(), s.end()); }
    etl::circular_buffer<char, BufferSize>& data() { return m_buffer; }
    const etl::circular_buffer<char, BufferSize>& data() const { return m_buffer; }
    void clear() { return m_buffer.clear(); }

private:
    etl::circular_buffer<char, BufferSize> m_buffer;
};

template<size_t N, typename Output>
class circular_ostream
{
public:
    template<typename T>
    circular_ostream& operator<<(const T& t)
    {
        m_tmpStream.clear();
        m_tmpStream << t;

        m_output.write(m_tmpStream.str());

        return *this;
    }

    size_t size() const { return m_output.data().size(); }
    void clear() { return m_output.clear(); }

    void to_string(etl::istring& str) const
    {
        str.assign(m_output.data().begin(), m_output.data().end());
    }

    size_t pop_to(etl::array_view<char> buffer)
    {
        auto it = buffer.begin();
        while(it != buffer.end() && !m_output.data().empty())
        {
            *it = m_output.data().front();
            m_output.data().pop();
            ++it;
        }

        return it - buffer.begin();
    }

private:
    embedded_ostream_base<N> m_tmpStream;
    Output m_output;
};
} // namespace embedded_io