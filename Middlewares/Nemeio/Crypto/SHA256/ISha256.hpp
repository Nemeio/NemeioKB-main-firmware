#pragma once

#include "etl/array_view.h"

class ISha256
{
public:
    ISha256() = default;
    virtual ~ISha256() = default;

    virtual bool init() = 0;
    virtual bool append(etl::array_view<const std::byte> data) = 0;
    virtual bool finish() = 0;
    virtual void reset() = 0;

    virtual etl::array_view<const std::byte> getDigest() const = 0;
};