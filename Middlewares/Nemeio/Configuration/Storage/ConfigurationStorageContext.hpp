#pragma once

#include "IUserConfigurationStorage.hpp"
#include "ILayoutStorage.hpp"

class ConfigurationStorageContext
{
public:
    ConfigurationStorageContext(IUserConfigurationStorage& userConfigurationStorage,
                                ILayoutStorage& layoutStorage)
        : m_userConfigurationStorage(userConfigurationStorage)
        , m_layoutStorage(layoutStorage)
    {
    }
    virtual ~ConfigurationStorageContext() = default;

    IUserConfigurationStorage& getUserConfigurationStorage() { return m_userConfigurationStorage; }
    ILayoutStorage& getLayoutStorage() { return m_layoutStorage; }

private:
    IUserConfigurationStorage& m_userConfigurationStorage;
    ILayoutStorage& m_layoutStorage;
};