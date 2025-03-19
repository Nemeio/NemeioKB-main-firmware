
#pragma once

#include "KeyboardParameters.hpp"
#include "etl/array_view.h"

class IKeyboardParametersCodec
{
public:
    IKeyboardParametersCodec() = default;
    virtual ~IKeyboardParametersCodec() = default;

    virtual bool decode(etl::array_view<const std::byte> data,
                        KeyboardParameters& keyboardParameters) const = 0;
    virtual ssize_t encode(const KeyboardParameters& keyboardParameters,
                           etl::array_view<std::byte> data) const = 0;
};
