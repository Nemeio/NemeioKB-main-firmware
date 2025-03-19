#include "LayoutWriter.hpp"
#include "ConfigurationStorageUtil.hpp"

using namespace configuration;

LayoutWriter::LayoutWriter(uuid::UuidView id,
                           fs::IFileSystem& fileSystem,
                           etl::string_view directoryPath,
                           ILayoutMetadataCodec& layoutMetadataCodec)
    : AbstractConfigurationFileWriter(id, fileSystem, directoryPath)
    , m_layoutMetadataCodec(layoutMetadataCodec)
{
}

Ret LayoutWriter::endWrite()
{
    // File must be closed before validating the package
    m_file.close();

    Ret ret = checkFile();

    if(ret != Ret::SUCCESS)
    {
        embedded_ostream path;
        getFilePath(path);

        m_fileSystem.remove(path.str().c_str());
    }

    return ret;
}

Ret LayoutWriter::move(etl::string_view newDirectoryPath)
{
    embedded_ostream oldPath;
    getFilePath(oldPath);

    m_directoryPath.assign(newDirectoryPath.begin(), newDirectoryPath.end());

    embedded_ostream newPath;
    getFilePath(newPath);

    int fsErr = m_fileSystem.rename(oldPath.str().c_str(), newPath.str().c_str());

    return ConfigurationStorageUtil::convertFsErrorCode(fsErr);
}

Ret LayoutWriter::checkFile()
{
    Ret ret = Ret::SUCCESS;
    embedded_ostream path;
    getFilePath(path);

    configuration::LayoutImagePackage package(m_fileSystem, path.str().c_str());

    if(int fsErr = package.mount(); fsErr != fs::FS_ERR_OK)
    {
        ret = fsErr == fs::FS_ERR_CORRUPT
                  ? Ret::INVALID_DATA
                  : ConfigurationStorageUtil::convertFsErrorCode(static_cast<fs::FSErrCode>(fsErr));
    }

    etl::string<fs::MAX_PATH_SIZE> metadataPath;
    if(ret == Ret::SUCCESS && !package.getMetadataPath(metadataPath))
    {
        ret = Ret::INVALID_DATA;
    }

    ssize_t readBytes = 0;
    etl::array<std::byte, configuration::MAX_METADATA_FILE_SIZE_BYTES> readBuffer;
    if(ret == Ret::SUCCESS)
    {
        fs::File f(package.getPackageFileSystem());
        f.open(metadataPath.data(), fs::OpenFlag::RDONLY);
        readBytes = f.read(readBuffer.data(), readBuffer.size());

        ret = readBytes == f.size() ? Ret::SUCCESS
                                    : ConfigurationStorageUtil::convertFsErrorCode(readBytes);
    }

    if(ret == Ret::SUCCESS)
    {
        OptionalReturn<configuration::LayoutMetadata> retLayoutMetadata(etl::nullopt);
        ret = m_layoutMetadataCodec.decode(etl::array_view<std::byte>{readBuffer.data(), readBytes},
                                           retLayoutMetadata)
                  ? Ret::SUCCESS
                  : Ret::INVALID_DATA;
    }

    return ret;
}

void LayoutWriter::getFileName(embedded_ostream& os) const
{
    os << m_id;
    os << configuration::LAYOUT_EXTENSION;
}