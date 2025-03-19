#pragma once

#include "IUserConfigurationStorage.hpp"
#include "IUserConfigurationCodec.hpp"
#include "ILayoutConfigurationCodec.hpp"
#include "ILayoutMetadataCodec.hpp"
#include "Layout.hpp"
#include "Mutex.hpp"
#include "UserConfigurationStorageObserver.hpp"
#include "ISharedBufferContainer.hpp"
#include "UserConfigurationListIterator.hpp"

inline constexpr size_t USER_CONFIGURATION_STORAGE_MAX_OBSERVERS = 1;

class UserConfigurationStorage : public IUserConfigurationStorage,
                                 public etl::observable<UserConfigurationStorageObserver,
                                                        USER_CONFIGURATION_STORAGE_MAX_OBSERVERS>
{
public:
    UserConfigurationStorage(fs::IFileSystem& fileSystem,
                             etl::string_view directoryPath,
                             IUserConfigurationCodec& userConfigurationCodec,
                             ILayoutConfigurationCodec& layoutConfigurationCodec,
                             ISharedBufferContainer& sharedBufferContainer);
    ~UserConfigurationStorage() final = default;

    configuration::Ret deleteUserConfiguration(uuid::UuidView id) final;
    void clear() final;

    bool isExistingUserConfiguration(uuid::UuidView id) final;
    configuration::Ret getUserConfiguration(
        uuid::UuidView id, etl::optional<configuration::UserConfiguration>& userConfiguration) final;
    configuration::Ret getLayoutConfiguration(
        uuid::UuidView userConfigurationId,
        uuid::UuidView layoutConfigurationId,
        etl::optional<configuration::LayoutConfiguration>& layoutConfiguration) final;

    etl::optional<etl::reference_wrapper<IUuidList::Iterator>> createIterator() final;
    void deleteIterator(IUuidList::Iterator& iterator) final;

    void getUserConfigurationPath(uuid::UuidView id, embedded_ostream& retPath) const final;
    fs::IFileSystem& getFileSystem() final { return m_fileSystem; };

private:
    static constexpr size_t MAX_ITERATORS = 2;

    fs::IFileSystem& m_fileSystem;
    etl::string<configuration::MAX_DIRECTORY_PATH_SIZE> m_directoryPath;
    IUserConfigurationCodec& m_userConfigurationCodec;
    ILayoutConfigurationCodec& m_layoutConfigurationCodec;
    ISharedBufferContainer& m_sharedBufferContainer;

    Mutex m_poolMutex;
    etl::pool<UserConfigurationListIterator, MAX_ITERATORS> m_iteratorPool;

    configuration::Ret readFile(fs::IFileSystem& fs,
                                etl::string_view inputPath,
                                etl::array_view<std::byte> data,
                                size_t& retSize);
    configuration::Ret readUserConfigurationFromFile(
        etl::string_view inputPath,
        uuid::UuidView id,
        etl::optional<configuration::UserConfiguration>& userConfiguration);
    configuration::Ret readLayoutConfigurationFromFile(
        etl::string_view inputPath,
        etl::optional<uuid::UuidView> id,
        OptionalReturn<configuration::LayoutConfiguration>& layoutConfiguration);

    void getUserConfigurationDirectoryPath(uuid::UuidView id, embedded_ostream& os) const;
    void getLayoutConfigurationPath(uuid::UuidView userConfigurationId,
                                    uuid::UuidView layoutConfigurationId,
                                    embedded_ostream& os) const;
};
