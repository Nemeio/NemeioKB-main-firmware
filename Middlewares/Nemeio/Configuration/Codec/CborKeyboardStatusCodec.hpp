#pragma once

#include "IKeyboardStatus.hpp"
#include "etl/array_view.h"
#include "etl/optional.h"
#include "qcbor/qcbor.h"

class CborKeyboardStatusCodec
{
public:
    static bool encodeToContextN(QCBOREncodeContext& context,
                                 const IKeyboardStatus& status,
                                 int64_t label);

private:
    enum class CborId
    {
        TRACE_LOG_SIZE = 1,
        TRACE_IS_ERROR_LOGGED = 2,
    };
};
