#include "CborLayoutConfigurationCodec.hpp"
#include "qcbor_util.hpp"

using namespace configuration;

bool CborLayoutConfigurationCodec::decode(
    etl::array_view<const std::byte> data,
    etl::optional<uuid::UuidView> id,
    OptionalReturn<LayoutConfiguration>& retLayoutConfiguration) const
{
    return decodeFromData(data, id, retLayoutConfiguration);
}

bool CborLayoutConfigurationCodec::decodeFromData(
    etl::array_view<const std::byte> data,
    etl::optional<uuid::UuidView> id,
    OptionalReturn<LayoutConfiguration>& retLayoutConfiguration)
{
    UsefulBufC inputBuffer = {data.data(), data.size()};
    QCBORDecodeContext context;
    QCBORDecode_Init(&context, inputBuffer, QCBOR_DECODE_MODE_NORMAL);

    bool ret = decodeFromContext(context, id, retLayoutConfiguration);

    if(QCBORDecode_Finish(&context) && retLayoutConfiguration.has_value())
    {
        retLayoutConfiguration.value().get().reset();
    }

    return ret;
}

bool CborLayoutConfigurationCodec::decodeFromContext(
    QCBORDecodeContext& context,
    etl::optional<uuid::UuidView> id,
    OptionalReturn<LayoutConfiguration>& retLayoutConfiguration)
{
    QCBORItem item;

    etl::array_view<const std::byte> layoutHashView;
    etl::optional<etl::array_view<const std::byte>> associatedHidLayoutConfigurationIdView;

    OperatingSystem os = OperatingSystem::UNDEFINED;
    etl::vector<KeySequence, LayoutConfiguration::MAX_WIN_SAS_SEQUENCES> winSas;
    etl::vector<KeySequence, LayoutConfiguration::MAX_CAPSLOCK_SEQUENCES> capslock;

    QCBORDecode_EnterMap(&context, &item);
    bool success = !QCBORDecode_GetAndResetError(&context);

    success = success && parseLayoutHash(context, layoutHashView)
              && parseAssociatedHidLayoutConfigurationId(context,
                                                         associatedHidLayoutConfigurationIdView)
              && parseOs(context, os) && parseWinSas(context, winSas)
              && parseCapslock(context, capslock);

    QCBORDecode_ExitMap(&context);
    success = !QCBORDecode_GetAndResetError(&context) && success;

    if(success && retLayoutConfiguration.has_value() && id.has_value())
    {
        retLayoutConfiguration.value()
            .get()
            .emplace(id.value(),
                     uuid::UuidView(layoutHashView),
                     associatedHidLayoutConfigurationIdView.has_value()
                         ? etl::optional<uuid::UuidView>(
                             uuid::UuidView(associatedHidLayoutConfigurationIdView.value()))
                         : etl::nullopt,
                     os,
                     winSas,
                     capslock);
    }

    return success;
}

bool CborLayoutConfigurationCodec::parseLayoutHash(QCBORDecodeContext& context,
                                                   etl::array_view<const std::byte>& layoutHashView)
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

bool CborLayoutConfigurationCodec::parseAssociatedHidLayoutConfigurationId(
    QCBORDecodeContext& context,
    etl::optional<etl::array_view<const std::byte>>& associatedHidLayoutConfigurationIdView)
{
    UsefulBufC associatedHidLayoutConfigurationIdBuffer;
    QCBORDecode_GetByteStringInMapN(&context,
                                    static_cast<int64_t>(
                                        CborId::ASSOCIATED_HID_LAYOUT_CONFIGURATION_ID),
                                    &associatedHidLayoutConfigurationIdBuffer);

    QCBORError qcborErr = QCBORDecode_GetAndResetError(&context);

    bool success = !qcborErr && associatedHidLayoutConfigurationIdBuffer.len == uuid::SIZE;

    if(success)
    {
        associatedHidLayoutConfigurationIdView
            .emplace(reinterpret_cast<const std::byte*>(
                         associatedHidLayoutConfigurationIdBuffer.ptr),
                     associatedHidLayoutConfigurationIdBuffer.len);
    }
    else
    {
        success = qcborErr == QCBOR_ERR_LABEL_NOT_FOUND;
    }

    if(!success && qcborErr == QCBOR_ERR_UNEXPECTED_TYPE)
    {
        QCBORDecode_GetNullInMapN(&context,
                                  static_cast<int64_t>(
                                      CborId::ASSOCIATED_HID_LAYOUT_CONFIGURATION_ID));
        qcborErr = QCBORDecode_GetAndResetError(&context);

        success = !qcborErr || (qcborErr == QCBOR_ERR_LABEL_NOT_FOUND);
    }

    return success;
}

bool CborLayoutConfigurationCodec::parseOs(QCBORDecodeContext& context, OperatingSystem& os)
{
    uint64_t osU64 = 0;
    QCBORDecode_GetUInt64InMapN(&context, static_cast<int64_t>(CborId::OPERATING_SYSTEM), &osU64);

    bool success = !QCBORDecode_GetAndResetError(&context)
                   && osU64 > static_cast<uint64_t>(OperatingSystem::UNDEFINED)
                   && osU64 < static_cast<uint64_t>(OperatingSystem::MAX);

    if(success)
    {
        os = static_cast<OperatingSystem>(osU64);
    }

    return success;
}

bool CborLayoutConfigurationCodec::parseWinSas(
    QCBORDecodeContext& context,
    etl::vector<KeySequence, LayoutConfiguration::MAX_WIN_SAS_SEQUENCES>& winSas)
{
    return parseKeySequence(context, winSas, CborId::WINSAS);
}

bool CborLayoutConfigurationCodec::parseCapslock(
    QCBORDecodeContext& context,
    etl::vector<KeySequence, LayoutConfiguration::MAX_CAPSLOCK_SEQUENCES>& capslock)
{
    return parseKeySequence(context, capslock, CborId::CAPSLOCK);
}

bool CborLayoutConfigurationCodec::parseKeySequence(QCBORDecodeContext& context,
                                                    etl::ivector<KeySequence>& sequence,
                                                    CborId label)
{
    auto addItem = [&sequence](QCBORDecodeContext& c)
    {
        bool addItemSuccess = false;
        if(!sequence.full())
        {
            sequence.emplace_back(KeySequence{});
            addItemSuccess = parseKeysInKeySequence(c, sequence);
        }

        return addItemSuccess;
    };

    qcbor::Ret ret = qcbor::Util::parseArrayFromN(context, static_cast<int64_t>(label), addItem);

    return ret == qcbor::Ret::SUCCESS || ret == qcbor::Ret::NO_ENT;
}


bool CborLayoutConfigurationCodec::parseKeysInKeySequence(QCBORDecodeContext& context,
                                                          etl::ivector<KeySequence>& sequence)
{
    auto addItem = [&sequence](QCBORDecodeContext& c)
    {
        uint64_t index;
        QCBORDecode_GetUInt64(&c, &index);
        bool addItemSuccess = !QCBORDecode_GetAndResetError(&c)
                              && index <= ButtonID::KEYBOARD_BUTTON_MAX_INDEX
                              && !sequence.back().full();

        if(addItemSuccess)
        {
            sequence.back().push_back(static_cast<KeyIntIndex>(index));
        }

        return addItemSuccess;
    };

    qcbor::Ret ret = qcbor::Util::parseArray(context, addItem);

    return ret == qcbor::Ret::SUCCESS && !sequence.back().empty();
}