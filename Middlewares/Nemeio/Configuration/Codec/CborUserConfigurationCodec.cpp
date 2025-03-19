#include "CborUserConfigurationCodec.hpp"
#include "qcbor_util.hpp"
#include "CborLayoutConfigurationDescriptorCodec.hpp"

using namespace configuration;

bool CborUserConfigurationCodec::decode(etl::array_view<const std::byte> data,
                                        UserConfiguration& retUserConfiguration) const
{
    UsefulBufC inputBuffer = {data.data(), data.size()};
    QCBORDecodeContext context;
    QCBORDecode_Init(&context, inputBuffer, QCBOR_DECODE_MODE_NORMAL);

    bool ret = decodeFromContext(context, retUserConfiguration);

    ret = !QCBORDecode_Finish(&context) && ret;

    return ret;
}

bool CborUserConfigurationCodec::decodeFromContext(QCBORDecodeContext& context,
                                                   UserConfiguration& retUserConfiguration)
{
    QCBORItem item;

    QCBORDecode_EnterMap(&context, &item);
    bool success = !QCBORDecode_GetAndResetError(&context);

    success = success && parseLayoutConfigurationDescriptorList(context, retUserConfiguration);

    QCBORDecode_ExitMap(&context);

    return success;
}

bool CborUserConfigurationCodec::parseLayoutConfigurationDescriptorList(
    QCBORDecodeContext& context, UserConfiguration& userConfiguration)
{
    auto addItem = [&userConfiguration](QCBORDecodeContext& c)
    {
        etl::optional<LayoutConfigurationDescriptor> layoutConfigurationDescriptor;
        auto retLayoutConfigurationDescriptor = createOptionalReturn<LayoutConfigurationDescriptor>(
            layoutConfigurationDescriptor);

        bool decodeRet = CborLayoutConfigurationDescriptorCodec::decodeFromContext(
            c, retLayoutConfigurationDescriptor);

        bool addItemSuccess = decodeRet && layoutConfigurationDescriptor.has_value()
                              && userConfiguration.addLayoutConfiguration(
                                  layoutConfigurationDescriptor.value());

        return addItemSuccess;
    };
    qcbor::Ret ret = qcbor::Util::parseArrayFromN(context,
                                                  static_cast<int64_t>(
                                                      CborId::LAYOUT_CONFIGURATION_DESCRIPTORS),
                                                  addItem);

    return ret == qcbor::Ret::SUCCESS;
}