#pragma once

#include "UserConfiguration.hpp"

class IUserConfigurationCodec
{
public:
    IUserConfigurationCodec() = default;
    virtual ~IUserConfigurationCodec() = default;

    virtual bool decode(etl::array_view<const std::byte> data,
                        configuration::UserConfiguration& retUserConfiguration) const = 0;
};