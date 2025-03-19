#include "CborKeyboardStatusCodec.hpp"

bool CborKeyboardStatusCodec::encodeToContextN(QCBOREncodeContext& context,
                                               const IKeyboardStatus& status,
                                               int64_t label)
{
    bool success = true;


    QCBOREncode_OpenMapInMapN(&context, label);
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::TRACE_LOG_SIZE),
                                status.getLogSize());
    QCBOREncode_AddBoolToMapN(&context,
                              static_cast<int64_t>(CborId::TRACE_IS_ERROR_LOGGED),
                              status.isErrorLogged());
    QCBOREncode_CloseMap(&context);

    success = success && !QCBOREncode_GetErrorState(&context);

    return success;
}