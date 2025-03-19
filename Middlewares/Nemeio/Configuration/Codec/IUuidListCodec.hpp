#pragma once

#include "etl/vector.h"
#include "Uuid.hpp"

class IUuidListCodec
{
public:
    IUuidListCodec() = default;
    virtual ~IUuidListCodec() = default;

    virtual ssize_t encode(etl::array_view<const uuid::Uuid> uuids,
                           etl::array_view<std::byte> output) = 0;
};
