#pragma once

#include "gmock/gmock.h"
#include "IUserConfigurationCodec.hpp"

class MockUserConfigurationCodec : public IUserConfigurationCodec
{
public:
    MOCK_METHOD(bool,
                decode,
                (etl::array_view<const std::byte> data,
                 configuration::UserConfiguration& retUserConfiguration),
                (const override));
};