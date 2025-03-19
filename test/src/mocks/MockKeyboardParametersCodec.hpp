#pragma once

#include "gmock/gmock.h"
#include "IKeyboardParametersCodec.hpp"

class MockKeyboardParametersCodec : public IKeyboardParametersCodec
{
public:
    MOCK_METHOD(bool,
                decode,
                (etl::array_view<const std::byte> data, KeyboardParameters& keyboardParameters),
                (const override));
    MOCK_METHOD(ssize_t,
                encode,
                (const KeyboardParameters& keyboardParameters, etl::array_view<std::byte> data),
                (const override));
};