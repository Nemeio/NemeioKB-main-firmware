#include "CborLayoutConfigurationDescriptorCodec.hpp"
#include "qcbor_util.hpp"

using namespace configuration;

bool CborLayoutConfigurationDescriptorCodec::decode(
    etl::array_view<const std::byte> data,
    OptionalReturn<LayoutConfigurationDescriptor>& retLayoutConfigurationDescriptor) const
{
    return CborLayoutConfigurationDescriptorCodec::decodeFromData(data,
                                                                  retLayoutConfigurationDescriptor);
}

bool CborLayoutConfigurationDescriptorCodec::decodeFromData(
    etl::array_view<const std::byte> data,
    OptionalReturn<LayoutConfigurationDescriptor>& retLayoutConfigurationDescriptor)
{
    UsefulBufC inputBuffer = {data.data(), data.size()};
    QCBORDecodeContext context;
    QCBORDecode_Init(&context, inputBuffer, QCBOR_DECODE_MODE_NORMAL);

    bool ret = decodeFromContext(context, retLayoutConfigurationDescriptor);

    if(QCBORDecode_Finish(&context) && retLayoutConfigurationDescriptor.has_value())
    {
        retLayoutConfigurationDescriptor.value().get().reset();
    }

    return ret;
}

bool CborLayoutConfigurationDescriptorCodec::decodeFromContext(
    QCBORDecodeContext& context,
    OptionalReturn<LayoutConfigurationDescriptor>& retLayoutConfigurationDescriptor)
{
    QCBORItem item;

    QCBORDecode_EnterMap(&context, &item);
    bool success = !QCBORDecode_GetAndResetError(&context);

    etl::array_view<const std::byte> idView;
    etl::array_view<const std::byte> layoutConfigurationHashView;
    etl::array_view<const std::byte> layoutHashView;
    success = success && parseLayoutConfigurationId(context, idView)
              && parseLayoutConfigurationHash(context, layoutConfigurationHashView)
              && parseLayoutHash(context, layoutHashView);

    QCBORDecode_ExitMap(&context);
    success = !QCBORDecode_GetAndResetError(&context) && success;

    if(success && retLayoutConfigurationDescriptor.has_value())
    {
        retLayoutConfigurationDescriptor.value().get().emplace(
            LayoutConfigurationDescriptor{uuid::Uuid(idView),
                                          uuid::Uuid(layoutConfigurationHashView),
                                          uuid::Uuid(layoutHashView)});
    }

    return success;
}

bool CborLayoutConfigurationDescriptorCodec::parseLayoutConfigurationId(
    QCBORDecodeContext& context, etl::array_view<const std::byte>& idView)
{
    UsefulBufC idBuffer;
    QCBORDecode_GetByteStringInMapN(&context, static_cast<int64_t>(CborId::ID), &idBuffer);

    bool success = !QCBORDecode_GetAndResetError(&context) && idBuffer.len == uuid::SIZE;

    if(success)
    {
        idView.assign(reinterpret_cast<const std::byte*>(idBuffer.ptr), idBuffer.len);
    }

    return success;
}

bool CborLayoutConfigurationDescriptorCodec::parseLayoutConfigurationHash(
    QCBORDecodeContext& context, etl::array_view<const std::byte>& hashView)
{
    UsefulBufC hashBuffer;
    QCBORDecode_GetByteStringInMapN(&context,
                                    static_cast<int64_t>(CborId::LAYOUT_CONFIGURATION_HASH),
                                    &hashBuffer);

    bool success = !QCBORDecode_GetAndResetError(&context) && hashBuffer.len == uuid::SIZE;

    if(success)
    {
        hashView.assign(reinterpret_cast<const std::byte*>(hashBuffer.ptr), hashBuffer.len);
    }

    return success;
}

bool CborLayoutConfigurationDescriptorCodec::parseLayoutHash(
    QCBORDecodeContext& context, etl::array_view<const std::byte>& layoutHashView)
{
    UsefulBufC layoutHashBuffer;
    QCBORDecode_GetByteStringInMapN(&context,
                                    static_cast<int64_t>(CborId::LAYOUT_HASH),
                                    &layoutHashBuffer);

    bool success = !QCBORDecode_GetAndResetError(&context) && layoutHashBuffer.len == uuid::SIZE;

    if(success)
    {
        layoutHashView.assign(reinterpret_cast<const std::byte*>(layoutHashBuffer.ptr),
                              layoutHashBuffer.len);
    }

    return success;
}