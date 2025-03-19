#pragma once

#include "etl/string_stream.h"

// This class reuses etl::string_stream and provides streaming functionality for char.
namespace embedded_io
{
static constexpr char endl[] = "\r\n";

class embedded_string_stream : public etl::string_stream
{
public:
    using etl::string_stream::string_stream;

    template<typename T>
    embedded_string_stream& operator<<(const T& t)
    {
        *static_cast<etl::string_stream*>(this) << t;

        return *this;
    }

    embedded_string_stream& operator<<(char c)
    {
        char str[2] = {c, '\0'};
        *this << str;
        return *this;
    }
};
} // namespace embedded_io