#include "Sha256.hpp"
#include "AutoLock.h"

Sha256::Sha256(ICRC32& crc32)
    : m_crc32(crc32)
{
}

bool Sha256::init()
{
    AutoLock crcLock(m_crc32);
    /* Set the size of the desired hash digest */
    m_context.mTagSize = m_digest.size();

    /* Set flag field to default value */
    m_context.mFlags = E_HASH_DEFAULT;

    return SHA256_Init(&m_context) == HASH_SUCCESS;
}

bool Sha256::append(etl::array_view<const std::byte> data)
{
    AutoLock crcLock(m_crc32);

    return SHA256_Append(&m_context, reinterpret_cast<const uint8_t*>(data.data()), data.size())
           == HASH_SUCCESS;
}

bool Sha256::finish()
{
    AutoLock crcLock(m_crc32);

    int32_t size = static_cast<int32_t>(m_digest.size());
    bool ret = SHA256_Finish(&m_context, reinterpret_cast<uint8_t*>(m_digest.data()), &size)
                   == HASH_SUCCESS
               && size == m_digest.size();

    memset(&m_context, sizeof(m_context), 0);
    return ret;
}

void Sha256::reset()
{
    memset(&m_context, sizeof(m_context), 0);
    m_digest.fill(std::byte(0x00));
}