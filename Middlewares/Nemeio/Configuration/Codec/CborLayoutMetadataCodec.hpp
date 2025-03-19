
#pragma once

#include "ILayoutMetadataCodec.hpp"
#include "qcbor/qcbor.h"

class CborLayoutMetadataCodec : public ILayoutMetadataCodec
{
public:
    CborLayoutMetadataCodec() = default;
    ~CborLayoutMetadataCodec() final = default;

    bool decode(etl::array_view<const std::byte> data,
                OptionalReturn<configuration::LayoutMetadata>& retLayoutMetadata) const final;

    static bool decodeFromData(etl::array_view<const std::byte> data,
                               OptionalReturn<configuration::LayoutMetadata>& retLayoutMetadata);

    static bool decodeFromContext(QCBORDecodeContext& context,
                                  OptionalReturn<configuration::LayoutMetadata>& retLayoutMetadata);

private:
    enum class CborId
    {
        BACKGROUND = 1,
        FORMAT = 2
    };

    static bool parseBackground(QCBORDecodeContext& context,
                                ScreenDefs::BackgroundColor& background);
    static bool parseFormat(QCBORDecodeContext& context, ScreenDefs::PixelFormat& pixelFormat);
};
