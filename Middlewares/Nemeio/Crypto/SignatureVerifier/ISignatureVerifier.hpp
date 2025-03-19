#pragma once

#include "etl/array_view.h"

class ISignatureVerifier
{
public:
    ISignatureVerifier() = default;
    virtual ~ISignatureVerifier() = default;

    virtual bool init() = 0;
    virtual bool appendDataToVerify(etl::array_view<const std::byte> data) = 0;
    virtual bool verify(etl::array_view<const std::byte> signature) = 0;
};