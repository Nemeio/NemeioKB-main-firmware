#pragma once

#include "ISha256.hpp"
#include "IEcdsaP256.hpp"
#include "ISignatureVerifier.hpp"

class EcdsaSignatureVerifier : public ISignatureVerifier
{
public:
    EcdsaSignatureVerifier(ISha256& sha256, IEcdsaP256& ecdsaP256);
    ~EcdsaSignatureVerifier() final = default;

    bool init() final;
    bool appendDataToVerify(etl::array_view<const std::byte> data) final;
    bool verify(etl::array_view<const std::byte> signature) final;

private:
    ISha256& m_sha256;
    IEcdsaP256& m_ecdsaP256;
};