#include "qcbor_util.hpp"

using namespace qcbor;

Ret Util::parseBoolFromN(QCBORDecodeContext& context, int64_t label, etl::optional<bool>& value)
{
    bool parsedValue;
    QCBORDecode_GetBoolInMapN(&context, label, &parsedValue);

    QCBORError qcborErr = QCBORDecode_GetAndResetError(&context);
    qcbor::Ret ret = qcbor::Ret::ERROR;
    if(!qcborErr)
    {
        ret = qcbor::Ret::SUCCESS;
        value.emplace(parsedValue);
    }
    else if(qcborErr == QCBOR_ERR_LABEL_NOT_FOUND)
    {
        ret = qcbor::Ret::NO_ENT;
    }

    return ret;
}

size_t qcbor::decodeGetArraySizeFromMapN(QCBORDecodeContext& context, int64_t nLabel)
{
    QCBORItem arrayItem;
    QCBORDecode_GetItemInMapN(&context, nLabel, QCBOR_TYPE_ARRAY, &arrayItem);

    return arrayItem.val.uCount;
}