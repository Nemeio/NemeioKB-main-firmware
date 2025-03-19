#pragma once

#include "qcbor/qcbor_spiffy_decode.h"
#include "etl/optional.h"
#include "etl/limits.h"

namespace qcbor
{
enum class Ret
{
    SUCCESS,
    ERROR,
    NO_ENT,
};
// QCBOR API does not allow to get the array size with QCBORDecode_EnterArrayFromMapN
size_t decodeGetArraySizeFromMapN(QCBORDecodeContext& context, int64_t nLabel);
class Util
{
public:
    template<typename UintType>
    static Ret parseUintFromN(QCBORDecodeContext& context,
                              int64_t label,
                              etl::optional<UintType>& value);

    template<typename UintType>
    static Ret parseUint(QCBORDecodeContext& context, etl::optional<UintType>& value);

    static Ret parseBoolFromN(QCBORDecodeContext& context,
                              int64_t label,
                              etl::optional<bool>& value);

    template<typename AddItemFunc>
    static Ret parseArrayFromN(QCBORDecodeContext& context, int64_t label, AddItemFunc addItemFunc);

    template<typename AddItemFunc>
    static Ret parseArray(QCBORDecodeContext& context, AddItemFunc addItemFunc);

private:
    template<typename UintType>
    static bool isUint(uint64_t valueU64);
};
} // namespace qcbor


template<typename UintType>
qcbor::Ret qcbor::Util::parseUintFromN(QCBORDecodeContext& context,
                                       int64_t label,
                                       etl::optional<UintType>& value)
{
    uint64_t valueU64 = 0;
    QCBORDecode_GetUInt64InMapN(&context, label, &valueU64);

    QCBORError qcborErr = QCBORDecode_GetAndResetError(&context);
    qcbor::Ret ret = qcbor::Ret::ERROR;
    if(!qcborErr && isUint<UintType>(valueU64))
    {
        ret = qcbor::Ret::SUCCESS;
        value.emplace(static_cast<UintType>(valueU64));
    }
    else if(qcborErr == QCBOR_ERR_LABEL_NOT_FOUND)
    {
        ret = qcbor::Ret::NO_ENT;
    }

    return ret;
}

template<typename UintType>
qcbor::Ret qcbor::Util::parseUint(QCBORDecodeContext& context, etl::optional<UintType>& value)
{
    uint64_t valueU64 = 0;
    QCBORDecode_GetUInt64(&context, &valueU64);

    qcbor::Ret ret = qcbor::Ret::ERROR;
    if(!QCBORDecode_GetAndResetError(&context) && isUint<UintType>(valueU64))
    {
        ret = qcbor::Ret::SUCCESS;
        value.emplace(static_cast<UintType>(valueU64));
    }

    return ret;
}

template<typename AddItemFunc>
qcbor::Ret qcbor::Util::parseArrayFromN(QCBORDecodeContext& context,
                                        int64_t label,
                                        AddItemFunc addItemFunc)
{
    size_t arraySize = qcbor::decodeGetArraySizeFromMapN(context, label);

    if(QCBORError qcborErr = QCBORDecode_GetAndResetError(&context); qcborErr)
    {
        return qcborErr == QCBOR_ERR_LABEL_NOT_FOUND ? qcbor::Ret::NO_ENT : qcbor::Ret::ERROR;
    }

    bool success = true;
    QCBORDecode_EnterArrayFromMapN(&context, label);

    for(size_t i = 0; i < arraySize; ++i)
    {
        success = addItemFunc(context);
        if(!success)
        {
            break;
        }
    }

    QCBORDecode_ExitArray(&context);

    success = !QCBORDecode_GetAndResetError(&context) && success;

    return success ? qcbor::Ret::SUCCESS : qcbor::Ret::ERROR;
}

template<typename AddItemFunc>
qcbor::Ret qcbor::Util::parseArray(QCBORDecodeContext& context, AddItemFunc addItemFunc)
{
    bool success = true;
    QCBORItem arrayItem;
    QCBORDecode_EnterArray(&context, &arrayItem);

    for(size_t i = 0; i < arrayItem.val.uCount; ++i)
    {
        success = addItemFunc(context);
        if(!success)
        {
            break;
        }
    }

    QCBORDecode_ExitArray(&context);

    success = !QCBORDecode_GetAndResetError(&context) && success;

    return success ? qcbor::Ret::SUCCESS : qcbor::Ret::ERROR;
}

template<typename UintType>
bool qcbor::Util::isUint(uint64_t valueU64)
{
    return valueU64 <= etl::numeric_limits<UintType>::max();
}