#pragma once

#include "ConfigurationCommon.hpp"
#include "Uuid.hpp"
#include "etl/optional.h"

class IConfigurationSwitcher
{
public:
    IConfigurationSwitcher() = default;
    virtual ~IConfigurationSwitcher() = default;

    virtual configuration::Ret selectUserConfiguration(
        uuid::UuidView userConfigurationId, etl::optional<uuid::UuidView> layoutConfigurationId) = 0;
    virtual configuration::Ret selectLayoutConfiguration(uuid::UuidView id) = 0;
    virtual configuration::Ret setNextConfiguration() = 0;
};
