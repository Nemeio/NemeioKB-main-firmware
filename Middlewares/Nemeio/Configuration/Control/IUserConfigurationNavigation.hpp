#pragma once

#include "Uuid.hpp"
#include "ConfigurationCommon.hpp"
#include "etl/optional.h"

class IUserConfigurationNavigation
{
public:
    IUserConfigurationNavigation() = default;
    virtual ~IUserConfigurationNavigation() = default;

    enum class SwitchDirection
    {
        BACKWARD,
        FORWARD,
    };

    virtual configuration::Ret enableCustom() = 0;
    virtual configuration::Ret disableCustom() = 0;
    virtual configuration::Ret applyFirst() = 0;
    virtual configuration::Ret applyNext(SwitchDirection direction) = 0;
    virtual configuration::Ret applyId(uuid::UuidView id) = 0;

    virtual etl::optional<uuid::UuidView> getCurrentUserConfigurationId() const = 0;
};