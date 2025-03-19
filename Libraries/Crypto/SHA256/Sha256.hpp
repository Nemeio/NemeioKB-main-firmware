#pragma once

#include "ISha256.hpp"
#include "crypto.h"
#include "ICRC32.hpp"

class Sha256 : public ISha256
{
public:
    explicit Sha256(ICRC32& crc32);
    ~Sha256() final = default;

    bool init() final;
    bool append(etl::array_view<const std::byte> data) final;
    bool finish() final;
    void reset() final;

    etl::array_view<const std::byte> getDigest() const final { return m_digest; }

private:
    // The CRC32 is used by the crypto library
    ICRC32& m_crc32;
    SHA256ctx_stt m_context;
    etl::array<std::byte, CRL_SHA256_SIZE> m_digest;
};