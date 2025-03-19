#pragma once

#include "LayoutConfiguration.hpp"
#include "OptionalReturn.hpp"

class ILayoutConfigurationCodec
{
public:
    ILayoutConfigurationCodec() = default;
    virtual ~ILayoutConfigurationCodec() = default;

    virtual bool decode(
        etl::array_view<const std::byte> data,
        etl::optional<uuid::UuidView> id,
        OptionalReturn<configuration::LayoutConfiguration>& retLayoutConfiguration) const = 0;
};