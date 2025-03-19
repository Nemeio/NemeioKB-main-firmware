
#include "ConfigurationStorageAssembly.hpp"

ConfigurationStorageAssembly::ConfigurationStorageAssembly(
    fs::IFileSystem& configurationFileSystem,
    fs::IFileSystem& factoryConfigurationFileSystem,
    ISharedBufferContainer& sharedBufferContainer)
    : m_userConfigurationWritersContainer(configurationFileSystem,
                                          m_userConfigurationCodec,
                                          m_layoutConfigurationCodec,
                                          sharedBufferContainer)
    , m_factoryUserConfigurationWritersContainer(factoryConfigurationFileSystem,
                                                 m_userConfigurationCodec,
                                                 m_layoutConfigurationCodec,
                                                 sharedBufferContainer)
    , m_layoutWritersContainer(configurationFileSystem, m_layoutMetadataCodec)
    , m_factoryLayoutWritersContainer(factoryConfigurationFileSystem, m_layoutMetadataCodec)
    , m_userConfigurationStorage(configurationFileSystem,
                                 configuration::VALID_USER_CONFIGURATION_DIRECTORY,
                                 m_userConfigurationCodec,
                                 m_layoutConfigurationCodec,
                                 sharedBufferContainer)
    , m_factoryUserConfigurationStorage(factoryConfigurationFileSystem,
                                        configuration::VALID_FACTORY_USER_CONFIGURATION_DIRECTORY,
                                        m_userConfigurationCodec,
                                        m_layoutConfigurationCodec,
                                        sharedBufferContainer)
    , m_layoutStorage(configurationFileSystem,
                      configuration::LAYOUT_DIRECTORY,
                      m_layoutMetadataCodec)
    , m_factoryLayoutStorage(configurationFileSystem,
                             configuration::FACTORY_LAYOUT_DIRECTORY,
                             m_layoutMetadataCodec)
    , m_configurationStorageContext(m_userConfigurationStorage, m_layoutStorage)
    , m_factoryConfigurationStorageContext(m_factoryUserConfigurationStorage, m_factoryLayoutStorage)
{
    configurationFileSystem.mkdir(configuration::VALID_USER_CONFIGURATION_DIRECTORY);
    configurationFileSystem.mkdir(configuration::LAYOUT_DIRECTORY);
    factoryConfigurationFileSystem.mkdir(configuration::VALID_FACTORY_USER_CONFIGURATION_DIRECTORY);
    factoryConfigurationFileSystem.mkdir(configuration::FACTORY_LAYOUT_DIRECTORY);
}


void ConfigurationStorageAssembly::registerToUserConfigurationStorageEvents(
    UserConfigurationStorageObserver& observer)
{
    m_userConfigurationStorage.add_observer(observer);
    m_factoryUserConfigurationStorage.add_observer(observer);
}
