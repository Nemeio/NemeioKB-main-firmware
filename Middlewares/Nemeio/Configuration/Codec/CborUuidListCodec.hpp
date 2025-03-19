#pragma once

#include "IUuidListCodec.hpp"
#include "qcbor/qcbor.h"

class CborUuidListCodec : public IUuidListCodec
{
public:
    CborUuidListCodec() = default;
    ~CborUuidListCodec() final = default;

    ssize_t encode(etl::array_view<const uuid::Uuid> uuids, etl::array_view<std::byte> output) final;

    static bool encodeToContext(QCBOREncodeContext& context,
                                etl::array_view<const uuid::Uuid> uuids);

private:
    enum class CborIdUuidList
    {
        LIST = 1
    };
    enum class CborIdUuidItem
    {
        UUID = 1
    };

    static bool encodeUuidItem(QCBOREncodeContext& context, uuid::UuidView uuid);
};