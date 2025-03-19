#pragma once

#include "etl/array_view.h"
#include "IKeyboardStatus.hpp"

class IKeepAliveRespCodec
{
public:
    IKeepAliveRespCodec() = default;
    virtual ~IKeepAliveRespCodec() = default;

    virtual ssize_t encode(const IKeyboardStatus& status, etl::array_view<std::byte> output) = 0;
};
