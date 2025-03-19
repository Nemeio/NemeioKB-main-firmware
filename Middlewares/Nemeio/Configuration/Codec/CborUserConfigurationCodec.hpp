#pragma once

#include "IUserConfigurationCodec.hpp"
#include "etl/array_view.h"
#include "etl/optional.h"
#include "qcbor/qcbor.h"

class CborUserConfigurationCodec : public IUserConfigurationCodec
{
public:
    CborUserConfigurationCodec() = default;
    ~CborUserConfigurationCodec() final = default;

    bool decode(etl::array_view<const std::byte> data,
                configuration::UserConfiguration& retUserConfiguration) const final;

    static bool decodeFromContext(QCBORDecodeContext& context,
                                  configuration::UserConfiguration& retUserConfiguration);

private:
    enum class CborId
    {
        LAYOUT_CONFIGURATION_DESCRIPTORS = 1
    };

    static bool parseLayoutConfigurationDescriptorList(
        QCBORDecodeContext& context, configuration::UserConfiguration& userConfiguration);
};
