#pragma once

#include "etl/array_view.h"

class IEcdsaP256
{
public:
    IEcdsaP256() = default;
    virtual ~IEcdsaP256() = default;

    virtual bool verify(etl::array_view<const std::byte> digest,
                        etl::array_view<const std::byte> signature) = 0;
};