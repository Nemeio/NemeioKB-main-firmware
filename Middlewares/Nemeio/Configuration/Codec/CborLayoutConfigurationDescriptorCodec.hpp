#pragma once

#include "UserConfiguration.hpp"
#include "etl/array_view.h"
#include "etl/optional.h"
#include "qcbor/qcbor.h"
#include "OptionalReturn.hpp"

class CborLayoutConfigurationDescriptorCodec
{
public:
    CborLayoutConfigurationDescriptorCodec() = default;
    virtual ~CborLayoutConfigurationDescriptorCodec() = default;

    bool decode(etl::array_view<const std::byte> data,
                OptionalReturn<configuration::LayoutConfigurationDescriptor>&
                    retLayoutConfigurationDescriptor) const;

    static bool decodeFromData(etl::array_view<const std::byte> data,
                               OptionalReturn<configuration::LayoutConfigurationDescriptor>&
                                   retLayoutConfigurationDescriptor);

    static bool decodeFromContext(QCBORDecodeContext& context,
                                  OptionalReturn<configuration::LayoutConfigurationDescriptor>&
                                      retLayoutConfigurationDescriptor);

private:
    enum class CborId
    {
        ID = 1,
        LAYOUT_CONFIGURATION_HASH = 2,
        LAYOUT_HASH = 3,
    };

    static bool parseLayoutConfigurationId(QCBORDecodeContext& context,
                                           etl::array_view<const std::byte>& idView);
    static bool parseLayoutConfigurationHash(QCBORDecodeContext& context,
                                             etl::array_view<const std::byte>& hashView);
    static bool parseLayoutHash(QCBORDecodeContext& context,
                                etl::array_view<const std::byte>& layoutHashView);
};
