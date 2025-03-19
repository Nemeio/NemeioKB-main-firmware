#pragma once
#include "gmock/gmock.h"
#include "ILayoutMetadataCodec.hpp"

class MockLayoutMetadataCodec : public ILayoutMetadataCodec
{
public:
    MOCK_METHOD(bool,
                decode,
                (etl::array_view<const std::byte> data,
                 OptionalReturn<configuration::LayoutMetadata>& retLayoutMetadata),
                (const override));
};
