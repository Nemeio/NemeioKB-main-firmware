#pragma once

#include "IUuidList.hpp"
#include "ConfigurationCommon.hpp"
#include "etl/vector.h"
#include "IUuidListCodec.hpp"

class UuidListEncoder
{
public:
    explicit UuidListEncoder(IUuidListCodec& uuidListCodec);
    virtual ~UuidListEncoder() = default;

    bool encode(IUuidList& list);
    ssize_t getEncodedSize() const { return m_encodedSize; }
    etl::array_view<std::byte> buffer() { return m_buffer; }

private:
    static constexpr size_t BUFFER_SIZE = 1024;
    static constexpr size_t MAX_LIST_SIZE = etl::max(configuration::MAX_LAYOUTS,
                                                     configuration::MAX_USER_CONFIGURATIONS);

    etl::array<std::byte, BUFFER_SIZE> m_buffer;
    etl::vector<uuid::Uuid, MAX_LIST_SIZE> m_vectorList;
    ssize_t m_encodedSize{0};
    IUuidListCodec& m_uuidListCodec;
};