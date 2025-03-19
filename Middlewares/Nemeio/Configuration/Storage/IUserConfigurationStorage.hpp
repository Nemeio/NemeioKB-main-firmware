#pragma once

#include "fs.hpp"
#include "UserConfiguration.hpp"
#include "ConfigurationCommon.hpp"
#include "IUuidList.hpp"

class IUserConfigurationStorage : public IUuidList
{
public:
    IUserConfigurationStorage() = default;
    ~IUserConfigurationStorage() override = default;

    virtual configuration::Ret deleteUserConfiguration(uuid::UuidView id) = 0;
    virtual void clear() = 0;

    virtual bool isExistingUserConfiguration(uuid::UuidView id) = 0;
    virtual configuration::Ret getUserConfiguration(
        uuid::UuidView id, etl::optional<configuration::UserConfiguration>& userConfiguration) = 0;
    virtual configuration::Ret getLayoutConfiguration(
        uuid::UuidView userConfigurationId,
        uuid::UuidView layoutConfigurationId,
        etl::optional<configuration::LayoutConfiguration>& layoutConfiguration) = 0;

    virtual void getUserConfigurationPath(uuid::UuidView id, embedded_ostream& retPath) const = 0;
    virtual fs::IFileSystem& getFileSystem() = 0;
};
