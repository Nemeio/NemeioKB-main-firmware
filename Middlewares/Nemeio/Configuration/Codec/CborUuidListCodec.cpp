#include "CborUuidListCodec.hpp"

ssize_t CborUuidListCodec::encode(etl::array_view<const uuid::Uuid> uuids,
                                  etl::array_view<std::byte> output)
{
    UsefulBuf outputBuffer = {output.data(), output.size()};
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, outputBuffer);

    encodeToContext(context, uuids);

    size_t cborSize;
    QCBORError err = QCBOREncode_FinishGetSize(&context, &cborSize);
    ssize_t ret = -1;
    if(!err && cborSize <= etl::numeric_limits<ssize_t>::max())
    {
        ret = static_cast<ssize_t>(cborSize);
    }

    return ret;
}

bool CborUuidListCodec::encodeToContext(QCBOREncodeContext& context,
                                        etl::array_view<const uuid::Uuid> uuids)
{
    bool success = true;

    QCBOREncode_OpenMap(&context);

    QCBOREncode_OpenArrayInMapN(&context, static_cast<uint64_t>(CborIdUuidList::LIST));
    for(auto& uuid: uuids)
    {
        success = encodeUuidItem(context, uuid::UuidView(uuid));
        if(!success)
        {
            break;
        }
    }
    QCBOREncode_CloseArray(&context);
    QCBOREncode_CloseMap(&context);

    success = success && !QCBOREncode_GetErrorState(&context);

    return success;
}

bool CborUuidListCodec::encodeUuidItem(QCBOREncodeContext& context, uuid::UuidView uuid)
{
    QCBOREncode_OpenMap(&context);

    UsefulBufC buffer = {uuid.get().data(), uuid.get().size()};
    QCBOREncode_AddBytesToMapN(&context, static_cast<uint64_t>(CborIdUuidItem::UUID), buffer);

    bool success = !QCBOREncode_GetErrorState(&context);

    QCBOREncode_CloseMap(&context);

    success = success && !QCBOREncode_GetErrorState(&context);

    return success;
}