#include "UserConfigurationStorage.hpp"
#include "ConfigurationStorageUtil.hpp"
#include "AutoLock.h"
#include "SharedBufferIds.hpp"

using namespace configuration;

UserConfigurationStorage::UserConfigurationStorage(
    fs::IFileSystem& fileSystem,
    etl::string_view directoryPath,
    IUserConfigurationCodec& userConfigurationCodec,
    ILayoutConfigurationCodec& layoutConfigurationCodec,
    ISharedBufferContainer& sharedBufferContainer)
    : m_fileSystem(fileSystem)
    , m_directoryPath(directoryPath)
    , m_userConfigurationCodec(userConfigurationCodec)
    , m_layoutConfigurationCodec(layoutConfigurationCodec)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

bool UserConfigurationStorage::isExistingUserConfiguration(uuid::UuidView id)
{
    embedded_ostream path;
    getUserConfigurationPath(id, path);

    return ConfigurationStorageUtil::isExistingPath(m_fileSystem, path.str().c_str());
}

configuration::Ret UserConfigurationStorage::getUserConfiguration(
    uuid::UuidView id, etl::optional<configuration::UserConfiguration>& userConfiguration)
{
    embedded_ostream path;
    getUserConfigurationPath(id, path);

    return readUserConfigurationFromFile(path.str().c_str(), id, userConfiguration);
}

configuration::Ret UserConfigurationStorage::getLayoutConfiguration(
    uuid::UuidView userConfigurationId,
    uuid::UuidView layoutConfigurationId,
    etl::optional<configuration::LayoutConfiguration>& layoutConfiguration)
{
    etl::optional optionalId(layoutConfigurationId);
    embedded_ostream path;
    getLayoutConfigurationPath(userConfigurationId, layoutConfigurationId, path);

    OptionalReturn<configuration::LayoutConfiguration> retUserConfiguration = createOptionalReturn(
        layoutConfiguration);

    return readLayoutConfigurationFromFile(path.str().c_str(), optionalId, retUserConfiguration);
}

etl::optional<etl::reference_wrapper<IUuidList::Iterator>> UserConfigurationStorage::createIterator()
{
    etl::optional<etl::reference_wrapper<IUuidList::Iterator>> ret;
    AutoLock autoLock(m_poolMutex);

    if(UserConfigurationListIterator* it = m_iteratorPool.create(m_fileSystem, m_directoryPath);
       it != nullptr)
    {
        ret.emplace(*it);
    }

    return ret;
}

void UserConfigurationStorage::deleteIterator(IUuidList::Iterator& iterator)
{
    AutoLock autoLock(m_poolMutex);
    m_iteratorPool.destroy(&iterator);
}

void UserConfigurationStorage::getUserConfigurationPath(uuid::UuidView id,
                                                        embedded_ostream& retPath) const
{
    getUserConfigurationDirectoryPath(id, retPath);
    retPath << '/' << configuration::USER_CONFIGURATION_FILENAME;
}

Ret UserConfigurationStorage::readFile(fs::IFileSystem& fs,
                                       etl::string_view inputPath,
                                       etl::array_view<std::byte> data,
                                       size_t& retSize)
{
    fs::File f(fs);

    if(int fsErr = f.open(inputPath.data(), fs::OpenFlag::RDONLY); fsErr != fs::FS_ERR_OK)
    {
        return ConfigurationStorageUtil::convertFsErrorCode(fsErr);
    }

    ssize_t readBytes = f.read(data.data(), data.size());

    Ret ret = readBytes == f.size() ? Ret::SUCCESS
                                    : ConfigurationStorageUtil::convertFsErrorCode(readBytes);

    if(ret == Ret::SUCCESS)
    {
        retSize = readBytes;
    }

    return ret;
}

Ret UserConfigurationStorage::readUserConfigurationFromFile(
    etl::string_view inputPath,
    uuid::UuidView id,
    etl::optional<configuration::UserConfiguration>& userConfiguration)
{
    ISharedBuffer& readBuffer = m_sharedBufferContainer.getSharedBuffer(
        shared_buffer_id::CONFIGURATION_SHARED_BUFFER_ID);
    AutoLock autoLock(readBuffer);

    size_t retSize = 0;
    Ret ret = readFile(m_fileSystem, inputPath, readBuffer.buffer(), retSize);

    if(ret == Ret::SUCCESS)
    {
        userConfiguration.emplace(id);
        ret = m_userConfigurationCodec.decode(etl::array_view<std::byte>{readBuffer.buffer().data(),
                                                                         retSize},
                                              userConfiguration.value())
                  ? Ret::SUCCESS
                  : Ret::INVALID_DATA;
    }

    return ret;
}

Ret UserConfigurationStorage::readLayoutConfigurationFromFile(
    etl::string_view inputPath,
    etl::optional<uuid::UuidView> id,
    OptionalReturn<configuration::LayoutConfiguration>& layoutConfiguration)
{
    ISharedBuffer& readBuffer = m_sharedBufferContainer.getSharedBuffer(
        shared_buffer_id::CONFIGURATION_SHARED_BUFFER_ID);
    AutoLock autoLock(readBuffer);

    size_t retSize = 0;
    Ret ret = readFile(m_fileSystem, inputPath, readBuffer.buffer(), retSize);

    if(ret == Ret::SUCCESS)
    {
        ret = m_layoutConfigurationCodec
                      .decode(etl::array_view<std::byte>{readBuffer.buffer().data(), retSize},
                              id,
                              layoutConfiguration)
                  ? Ret::SUCCESS
                  : Ret::INVALID_DATA;
    }

    return ret;
}

Ret UserConfigurationStorage::deleteUserConfiguration(uuid::UuidView id)
{
    embedded_ostream path;
    getUserConfigurationDirectoryPath(id, path);
    int err = m_fileSystem.remove_recursive(path.str().c_str());

    notify_observers(UserConfigurationDeleteEvent{id});

    return ConfigurationStorageUtil::convertFsErrorCode(static_cast<fs::FSErrCode>(err));
}

void UserConfigurationStorage::clear()
{
    m_fileSystem.remove_recursive(m_directoryPath.data());
}

void UserConfigurationStorage::getUserConfigurationDirectoryPath(uuid::UuidView id,
                                                                 embedded_ostream& os) const
{
    os << m_directoryPath.data() << '/';
    os << id;
}

void UserConfigurationStorage::getLayoutConfigurationPath(uuid::UuidView userConfigurationId,
                                                          uuid::UuidView layoutConfigurationId,
                                                          embedded_ostream& os) const
{
    getUserConfigurationDirectoryPath(userConfigurationId, os);
    os << '/';
    os << layoutConfigurationId;
    os << configuration::LAYOUT_CONFIGURATION_EXTENSION;
}