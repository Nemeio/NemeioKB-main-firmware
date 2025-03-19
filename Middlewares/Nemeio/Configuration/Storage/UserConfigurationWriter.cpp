#include "UserConfigurationWriter.hpp"
#include "ConfigurationStorageUtil.hpp"
#include "SharedBufferIds.hpp"

using namespace configuration;

UserConfigurationWriter::UserConfigurationWriter(uuid::UuidView id,
                                                 fs::IFileSystem& fileSystem,
                                                 etl::string_view directoryPath,
                                                 IUserConfigurationCodec& userConfigurationCodec,
                                                 ILayoutConfigurationCodec& layoutConfigurationCodec,
                                                 ISharedBufferContainer& sharedBufferContainer)
    : AbstractUserConfigurationWriter(id, fileSystem, directoryPath)
    , m_userConfigurationCodec(userConfigurationCodec)
    , m_layoutConfigurationCodec(layoutConfigurationCodec)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

Ret UserConfigurationWriter::initWrite()
{
    m_fileSystem.remove_recursive(m_directoryPath.data());
    m_fileSystem.mkdir(m_directoryPath.data());

    return AbstractUserConfigurationWriter::initWrite();
}

Ret UserConfigurationWriter::endWrite()
{
    Ret ret = checkFile();

    m_file.close();

    if(ret != Ret::SUCCESS)
    {
        m_layoutConfigurationWriter.reset();
        m_fileSystem.remove_recursive(m_directoryPath.data());
    }

    return ret;
}

Ret UserConfigurationWriter::move(etl::string_view newDirectoryPath)
{
    Ret ret = ConfigurationStorageUtil::renameBackupExisting(m_fileSystem,
                                                             m_directoryPath.data(),
                                                             newDirectoryPath.data());

    m_directoryPath.assign(newDirectoryPath.begin(), newDirectoryPath.end());

    return ret;
}

IConfigurationFileWriter& UserConfigurationWriter::createLayoutConfigurationWriter(uuid::UuidView id)
{
    m_layoutConfigurationWriter.emplace(id,
                                        m_fileSystem,
                                        m_directoryPath,
                                        m_layoutConfigurationCodec,
                                        m_sharedBufferContainer);

    return m_layoutConfigurationWriter.value();
}

Ret UserConfigurationWriter::checkFile()
{
    ISharedBuffer& readBuffer = m_sharedBufferContainer.getSharedBuffer(
        shared_buffer_id::CONFIGURATION_SHARED_BUFFER_ID);
    AutoLock autoLock(readBuffer);

    etl::fill(readBuffer.buffer().begin(), readBuffer.buffer().end(), std::byte(0));

    m_file.rewind();

    ssize_t readBytes = m_file.read(readBuffer.buffer().data(), readBuffer.buffer().size());
    Ret ret = readBytes == m_file.size() ? Ret::SUCCESS
                                         : ConfigurationStorageUtil::convertFsErrorCode(readBytes);

    if(ret == Ret::SUCCESS)
    {
        UserConfiguration retUserConfiguration(uuid::UuidView{m_id});
        ret = m_userConfigurationCodec.decode(etl::array_view<std::byte>{readBuffer.buffer().data(),
                                                                         readBytes},
                                              retUserConfiguration)
                  ? Ret::SUCCESS
                  : Ret::INVALID_DATA;
    }

    return ret;
}

void UserConfigurationWriter::getFileName(embedded_ostream& os) const
{
    os << USER_CONFIGURATION_FILENAME;
}