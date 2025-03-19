#pragma once

#include "IKeepAliveRespCodec.hpp"
#include "etl/array_view.h"
#include "qcbor/qcbor.h"

class CborKeepAliveRespCodec : public IKeepAliveRespCodec
{
public:
    CborKeepAliveRespCodec() = default;
    ~CborKeepAliveRespCodec() final = default;

    ssize_t encode(const IKeyboardStatus& status, etl::array_view<std::byte> output) final;

    static bool encodeToContext(QCBOREncodeContext& context, const IKeyboardStatus& status);

private:
    enum class CborId
    {
        KEYBOARD_STATUS = 1,
    };
};
