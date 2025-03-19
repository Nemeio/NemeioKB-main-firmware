#include "CborKeepAliveRespCodec.hpp"
#include "CborKeyboardStatusCodec.hpp"

ssize_t CborKeepAliveRespCodec::encode(const IKeyboardStatus& status,
                                       etl::array_view<std::byte> output)
{
    UsefulBuf outputBuffer = {output.data(), output.size()};
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, outputBuffer);

    encodeToContext(context, status);

    size_t cborSize;
    QCBORError err = QCBOREncode_FinishGetSize(&context, &cborSize);
    ssize_t ret = -1;
    if(!err && cborSize <= etl::numeric_limits<ssize_t>::max())
    {
        ret = static_cast<ssize_t>(cborSize);
    }

    return ret;
}

bool CborKeepAliveRespCodec::encodeToContext(QCBOREncodeContext& context,
                                             const IKeyboardStatus& status)
{
    bool success = true;

    QCBOREncode_OpenMap(&context);
    CborKeyboardStatusCodec::encodeToContextN(context,
                                              status,
                                              static_cast<int64_t>(CborId::KEYBOARD_STATUS));
    QCBOREncode_CloseMap(&context);

    success = success && !QCBOREncode_GetErrorState(&context);

    return success;
}