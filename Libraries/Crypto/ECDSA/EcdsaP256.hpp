#pragma once

#include "IEcdsaP256.hpp"
#include "ICRC32.hpp"

class EcdsaP256 : public IEcdsaP256
{
public:
    EcdsaP256(etl::array_view<const std::byte> publicKey, ICRC32& crc32);
    ~EcdsaP256() final = default;

    bool verify(etl::array_view<const std::byte> digest,
                etl::array_view<const std::byte> signature) final;

private:
    etl::array_view<const std::byte> m_publicKey;
    // The CRC32 is used by the crypto library
    ICRC32& m_crc32;
    etl::array<std::byte, 2048> m_preallocatedBuffer;
};