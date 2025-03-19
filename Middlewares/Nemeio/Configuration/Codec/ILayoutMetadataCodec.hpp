
#pragma once

#include "Layout.hpp"
#include "etl/array_view.h"
#include "OptionalReturn.hpp"

class ILayoutMetadataCodec
{
public:
    ILayoutMetadataCodec() = default;
    virtual ~ILayoutMetadataCodec() = default;

    virtual bool decode(etl::array_view<const std::byte> data,
                        OptionalReturn<configuration::LayoutMetadata>& retLayoutMetadata) const = 0;
};
