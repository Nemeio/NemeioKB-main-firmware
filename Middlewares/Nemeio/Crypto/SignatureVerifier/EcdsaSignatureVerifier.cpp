#include "EcdsaSignatureVerifier.hpp"

EcdsaSignatureVerifier::EcdsaSignatureVerifier(ISha256& sha256, IEcdsaP256& ecdsaP256)
    : m_sha256(sha256)
    , m_ecdsaP256(ecdsaP256)
{
}

bool EcdsaSignatureVerifier::init()
{
    m_sha256.reset();
    return m_sha256.init();
}

bool EcdsaSignatureVerifier::appendDataToVerify(etl::array_view<const std::byte> data)
{
    return m_sha256.append(data);
}

bool EcdsaSignatureVerifier::verify(etl::array_view<const std::byte> signature)
{
    bool ret = m_sha256.finish();

    if(ret)
    {
        ret = m_ecdsaP256.verify(m_sha256.getDigest(), signature);
    }

    m_sha256.reset();

    return ret;
}