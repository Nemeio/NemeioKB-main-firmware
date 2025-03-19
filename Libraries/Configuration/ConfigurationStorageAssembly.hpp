#pragma once

#include "UserConfigurationStorage.hpp"
#include "LayoutStorage.hpp"
#include "CborUserConfigurationCodec.hpp"
#include "CborLayoutConfigurationCodec.hpp"
#include "CborLayoutMetadataCodec.hpp"
#include "UserConfigurationWritersContainer.hpp"
#include "SharedBuffer.hpp"
#include "LayoutWritersContainer.hpp"
#include "ConfigurationStorageContext.hpp"
#include "ISharedBufferContainer.hpp"

class ConfigurationStorageAssembly
{
public:
    ConfigurationStorageAssembly(fs::IFileSystem& configurationFileSystem,
                                 fs::IFileSystem& factoryConfigurationFileSystem,
                                 ISharedBufferContainer& sharedBufferContainer);
    virtual ~ConfigurationStorageAssembly() = default;

    void init();

    void registerToUserConfigurationStorageEvents(UserConfigurationStorageObserver& observer);

    UserConfigurationStorage& getUserConfigurationStorage() { return m_userConfigurationStorage; }

    UserConfigurationStorage& getFactoryUserConfigurationStorage()
    {
        return m_factoryUserConfigurationStorage;
    }

    ConfigurationStorageContext& getConfigurationStorageContext()
    {
        return m_configurationStorageContext;
    }

    ConfigurationStorageContext& getFactoryConfigurationStorageContext()
    {
        return m_factoryConfigurationStorageContext;
    }

    LayoutStorage& getLayoutStorage() { return m_layoutStorage; }

    LayoutStorage& getFactoryLayoutStorage() { return m_factoryLayoutStorage; }

    UserConfigurationWritersContainer& getUserConfigurationWritersContainer()
    {
        return m_userConfigurationWritersContainer;
    }

    UserConfigurationWritersContainer& getFactoryUserConfigurationWritersContainer()
    {
        return m_factoryUserConfigurationWritersContainer;
    }

    LayoutWritersContainer& getLayoutWritersContainer() { return m_layoutWritersContainer; }

    LayoutWritersContainer& getFactoryLayoutWritersContainer()
    {
        return m_factoryLayoutWritersContainer;
    }

private:
    static constexpr size_t MAX_FILE_SIZE = 1900;

    CborUserConfigurationCodec m_userConfigurationCodec;
    CborLayoutConfigurationCodec m_layoutConfigurationCodec;
    CborLayoutMetadataCodec m_layoutMetadataCodec;

    UserConfigurationWritersContainer m_userConfigurationWritersContainer;
    UserConfigurationWritersContainer m_factoryUserConfigurationWritersContainer;

    LayoutWritersContainer m_layoutWritersContainer;
    LayoutWritersContainer m_factoryLayoutWritersContainer;

    UserConfigurationStorage m_userConfigurationStorage;
    UserConfigurationStorage m_factoryUserConfigurationStorage;

    LayoutStorage m_layoutStorage;
    LayoutStorage m_factoryLayoutStorage;

    ConfigurationStorageContext m_configurationStorageContext;
    ConfigurationStorageContext m_factoryConfigurationStorageContext;
};
