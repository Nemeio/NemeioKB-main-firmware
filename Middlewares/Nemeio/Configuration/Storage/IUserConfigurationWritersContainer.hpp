#pragma once

#include "AbstractUserConfigurationWriter.hpp"
#include "etl/optional.h"
#include "fs.hpp"

class IUserConfigurationWritersContainer
{
public:
    IUserConfigurationWritersContainer() = default;
    virtual ~IUserConfigurationWritersContainer() = default;

    virtual etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>>
    createUserConfigurationWriter(uuid::UuidView id,
                                  fs::IFileSystem& fileSystem,
                                  etl::string_view directoryPath) = 0;

    virtual etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>>
    getUserConfigurationWriter(uuid::UuidView id) = 0;

    virtual void resetUserConfigurationWriter(uuid::UuidView id) = 0;
};