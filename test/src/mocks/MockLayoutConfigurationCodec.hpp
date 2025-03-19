#pragma once

#include "gmock/gmock.h"
#include "ILayoutConfigurationCodec.hpp"

class MockLayoutConfigurationCodec : public ILayoutConfigurationCodec
{
public:
    MOCK_METHOD(bool,
                decode,
                (etl::array_view<const std::byte> data,
                 etl::optional<uuid::UuidView> id,
                 OptionalReturn<configuration::LayoutConfiguration>& retLayoutConfiguration),
                (const override));
};