#include "CborLayoutMetadataCodec.hpp"
#include "qcbor/qcbor_spiffy_decode.h"

using namespace configuration;

bool CborLayoutMetadataCodec::decode(etl::array_view<const std::byte> data,
                                     OptionalReturn<LayoutMetadata>& retLayoutMetadata) const
{
    return CborLayoutMetadataCodec::decodeFromData(data, retLayoutMetadata);
}

bool CborLayoutMetadataCodec::decodeFromData(etl::array_view<const std::byte> data,
                                             OptionalReturn<LayoutMetadata>& retLayoutMetadata)
{
    UsefulBufC inputBuffer = {data.data(), data.size()};
    QCBORDecodeContext context;
    QCBORDecode_Init(&context, inputBuffer, QCBOR_DECODE_MODE_NORMAL);

    bool ret = decodeFromContext(context, retLayoutMetadata);

    if(QCBORDecode_Finish(&context) && retLayoutMetadata.has_value())
    {
        retLayoutMetadata.value().get().reset();
    }

    return ret;
}

bool CborLayoutMetadataCodec::decodeFromContext(QCBORDecodeContext& context,
                                                OptionalReturn<LayoutMetadata>& retLayoutMetadata)
{
    QCBORItem item;

    QCBORDecode_EnterMap(&context, &item);
    bool success = !QCBORDecode_GetAndResetError(&context);

    ScreenDefs::BackgroundColor background;
    ScreenDefs::PixelFormat format;

    success = success && parseBackground(context, background) && parseFormat(context, format);

    QCBORDecode_ExitMap(&context);
    success = !QCBORDecode_GetAndResetError(&context) && success;

    if(success && retLayoutMetadata.has_value())
    {
        retLayoutMetadata.value().get().emplace(LayoutMetadata{background, format});
    }

    return success;
}

bool CborLayoutMetadataCodec::parseBackground(QCBORDecodeContext& context,
                                              ScreenDefs::BackgroundColor& background)
{
    uint64_t backgroundU64 = 0;
    QCBORDecode_GetUInt64InMapN(&context, static_cast<int64_t>(CborId::BACKGROUND), &backgroundU64);

    bool success = !QCBORDecode_GetAndResetError(&context)
                   && backgroundU64 < static_cast<uint64_t>(ScreenDefs::BackgroundColor::MAX);

    if(success)
    {
        background = static_cast<ScreenDefs::BackgroundColor>(backgroundU64);
    }

    return success;
}

bool CborLayoutMetadataCodec::parseFormat(QCBORDecodeContext& context,
                                          ScreenDefs::PixelFormat& pixelFormat)
{
    uint64_t pixelFormatU64 = 0;
    QCBORDecode_GetUInt64InMapN(&context, static_cast<int64_t>(CborId::FORMAT), &pixelFormatU64);

    bool success = !QCBORDecode_GetAndResetError(&context);

    switch(static_cast<ScreenDefs::PixelFormat>(pixelFormatU64))
    {
    case ScreenDefs::PixelFormat::_1BPP:
    case ScreenDefs::PixelFormat::_2BPP:
    case ScreenDefs::PixelFormat::_JPEG:
        break;
    default:
        success = false;
        break;
    }

    if(success)
    {
        pixelFormat = static_cast<ScreenDefs::PixelFormat>(pixelFormatU64);
    }

    return success;
}