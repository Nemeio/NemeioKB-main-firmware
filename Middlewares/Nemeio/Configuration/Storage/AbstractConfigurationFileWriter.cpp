#include "AbstractConfigurationFileWriter.hpp"
#include "ConfigurationStorageUtil.hpp"

using namespace configuration;

AbstractConfigurationFileWriter::AbstractConfigurationFileWriter(uuid::UuidView id,
                                                                 fs::IFileSystem& fileSystem,
                                                                 etl::string_view directoryPath)
    : m_id(id)
    , m_fileSystem(fileSystem)
    , m_directoryPath(directoryPath)
    , m_file(fileSystem)
{
}

Ret AbstractConfigurationFileWriter::initWrite()
{
    embedded_ostream path;
    getFilePath(path);

    int fsErr = m_file.open(path.str().c_str(),
                            fs::OpenFlag::CREAT | fs::OpenFlag::RDWR | fs::OpenFlag::TRUNC);

    return ConfigurationStorageUtil::convertFsErrorCode(fsErr);
}

Ret AbstractConfigurationFileWriter::write(etl::array_view<const std::byte> data)
{
    int writtenBytes = m_file.write(data.data(), data.size());

    return writtenBytes == data.size() ? Ret::SUCCESS
                                       : ConfigurationStorageUtil::convertFsErrorCode(writtenBytes);
}

configuration::Ret AbstractConfigurationFileWriter::move(etl::string_view newDirectoryPath)
{
    embedded_ostream oldPath;
    getFilePath(oldPath);

    m_directoryPath.assign(newDirectoryPath.begin(), newDirectoryPath.end());

    embedded_ostream newPath;
    getFilePath(newPath);

    int fsErr = m_fileSystem.rename(oldPath.str().c_str(), newPath.str().c_str());

    return ConfigurationStorageUtil::convertFsErrorCode(fsErr);
}

void AbstractConfigurationFileWriter::getFilePath(embedded_ostream& os) const
{
    os << m_directoryPath.data() << '/';
    getFileName(os);
}