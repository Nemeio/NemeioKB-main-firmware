#include "LayoutConfigurationWriter.hpp"
#include "ConfigurationStorageUtil.hpp"
#include "SharedBufferIds.hpp"

using namespace configuration;

#include <cstdio>

LayoutConfigurationWriter::LayoutConfigurationWriter(
    uuid::UuidView id,
    fs::IFileSystem& fileSystem,
    etl::string_view directoryPath,
    ILayoutConfigurationCodec& layoutConfigurationCodec,
    ISharedBufferContainer& sharedBufferContainer)
    : AbstractConfigurationFileWriter(id, fileSystem, directoryPath)
    , m_layoutConfigurationCodec(layoutConfigurationCodec)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

Ret LayoutConfigurationWriter::endWrite()
{
    Ret ret = checkFile();

    m_file.close();

    if(ret != Ret::SUCCESS)
    {
        embedded_ostream path;
        getFilePath(path);

        m_fileSystem.remove(path.str().c_str());
    }

    return ret;
}

Ret LayoutConfigurationWriter::checkFile()
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
        OptionalReturn<LayoutConfiguration> retLayoutConfiguration(etl::nullopt);
        ret = m_layoutConfigurationCodec
                      .decode(etl::array_view<std::byte>{readBuffer.buffer().data(), readBytes},
                              uuid::UuidView(m_id),
                              retLayoutConfiguration)
                  ? Ret::SUCCESS
                  : Ret::INVALID_DATA;
    }

    return ret;
}

void LayoutConfigurationWriter::getFileName(embedded_ostream& os) const
{
    os << m_id;
    os << configuration::LAYOUT_CONFIGURATION_EXTENSION;
}