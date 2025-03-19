#pragma once

#include "ILayoutConfigurationCodec.hpp"
#include "etl/array_view.h"
#include "etl/optional.h"
#include "qcbor/qcbor.h"


class CborLayoutConfigurationCodec : public ILayoutConfigurationCodec
{
public:
    CborLayoutConfigurationCodec() = default;
    ~CborLayoutConfigurationCodec() final = default;

    bool decode(
        etl::array_view<const std::byte> data,
        etl::optional<uuid::UuidView> id,
        OptionalReturn<configuration::LayoutConfiguration>& retLayoutConfiguration) const final;

    static bool decodeFromData(
        etl::array_view<const std::byte> data,
        etl::optional<uuid::UuidView> id,
        OptionalReturn<configuration::LayoutConfiguration>& retLayoutConfiguration);

    static bool decodeFromContext(
        QCBORDecodeContext& context,
        etl::optional<uuid::UuidView> id,
        OptionalReturn<configuration::LayoutConfiguration>& retLayoutConfiguration);

private:
    enum class CborId
    {
        LAYOUT_HASH = 1,
        ASSOCIATED_HID_LAYOUT_CONFIGURATION_ID,
        OPERATING_SYSTEM,
        WINSAS,
        CAPSLOCK
    };

    static bool parseLayoutHash(QCBORDecodeContext& context,
                                etl::array_view<const std::byte>& layoutHashView);
    static bool parseAssociatedHidLayoutConfigurationId(
        QCBORDecodeContext& context,
        etl::optional<etl::array_view<const std::byte>>& associatedHidLayoutConfigurationIdView);
    static bool parseOs(QCBORDecodeContext& context, configuration::OperatingSystem& os);
    static bool parseWinSas(
        QCBORDecodeContext& context,
        etl::vector<configuration::KeySequence,
                    configuration::LayoutConfiguration::MAX_WIN_SAS_SEQUENCES>& winSas);
    static bool parseCapslock(
        QCBORDecodeContext& context,
        etl::vector<configuration::KeySequence,
                    configuration::LayoutConfiguration::MAX_CAPSLOCK_SEQUENCES>& capslock);
    static bool parseKeySequence(QCBORDecodeContext& context,
                                 etl::ivector<configuration::KeySequence>& sequence,
                                 CborId label);
    static bool parseKeysInKeySequence(QCBORDecodeContext& context,
                                       etl::ivector<configuration::KeySequence>& sequence);
};
