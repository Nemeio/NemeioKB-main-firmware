#pragma once

#include <string>
#include "ScreenDefs.hpp"
#include "LayoutConfiguration.hpp"
#include "Layout.hpp"

struct ConfigurationDisplayInfo
{
    etl::optional<configuration::Layout> layout;
    configuration::OperatingSystem operatingSystem;

    bool operator==(const ConfigurationDisplayInfo& info) const
    {
        return info.layout == layout && info.operatingSystem == operatingSystem;
    }
};