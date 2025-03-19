#include "LayoutStorage.hpp"
#include "ConfigurationStorageUtil.hpp"
#include "PackageFileSystem.hpp"
#include "etl/string.h"
#include "AutoLock.h"

using namespace configuration;

LayoutStorage::LayoutStorage(fs::IFileSystem& fileSystem,
                             etl::string_view directoryPath,
                             ILayoutMetadataCodec& layoutMetadataCodec)
    : m_fileSystem(fileSystem)
    , m_directoryPath(directoryPath)
    , m_layoutMetadataCodec(layoutMetadataCodec)
{
}

Ret LayoutStorage::deleteLayout(uuid::UuidView hash)
{
    embedded_ostream path;
    getLayoutPath(hash, path);

    int fsErr = m_fileSystem.remove(path.str().c_str());
    Ret ret = ConfigurationStorageUtil::convertFsErrorCode(static_cast<fs::FSErrCode>(fsErr));

    return ret;
}

void LayoutStorage::clear()
{
    m_fileSystem.remove_recursive(m_directoryPath.data());
}

configuration::Ret LayoutStorage::getLayout(uuid::UuidView hash,
                                            etl::optional<configuration::Layout>& layout)
{
    embedded_ostream path;
    getLayoutPath(hash, path);

    OptionalReturn<configuration::Layout> retLayout = createOptionalReturn(layout);

    return readLayoutFromFile(path.str().c_str(), hash, retLayout);
}

etl::optional<etl::reference_wrapper<IUuidList::Iterator>> LayoutStorage::createIterator()
{
    etl::optional<etl::reference_wrapper<IUuidList::Iterator>> ret;
    AutoLock autoLock(m_poolMutex);

    if(LayoutListIterator* it = m_iteratorPool.create(m_fileSystem, m_directoryPath); it != nullptr)
    {
        ret.emplace(*it);
    }

    return ret;
}

void LayoutStorage::deleteIterator(IUuidList::Iterator& iterator)
{
    AutoLock autoLock(m_poolMutex);
    m_iteratorPool.destroy(&iterator);
}

Ret LayoutStorage::readLayoutFromFile(etl::string_view inputPath,
                                      uuid::UuidView hash,
                                      OptionalReturn<configuration::Layout>& layout)
{
    if(!ConfigurationStorageUtil::isExistingPath(m_fileSystem, inputPath.data()))
    {
        return Ret::NOT_FOUND;
    }

    Ret ret = Ret::SUCCESS;
    configuration::LayoutImagePackage package(m_fileSystem, inputPath.data());

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

    size_t retSize = 0;
    etl::array<std::byte, configuration::MAX_METADATA_FILE_SIZE_BYTES> readBuffer;
    if(ret == Ret::SUCCESS)
    {
        ret = ConfigurationStorageUtil::readFile(package.getPackageFileSystem(),
                                                 metadataPath,
                                                 readBuffer,
                                                 retSize);
    }

    etl::optional<configuration::LayoutMetadata> layoutMetadata;
    OptionalReturn<configuration::LayoutMetadata> retLayoutMetadata =
        createOptionalReturn<configuration::LayoutMetadata>(layoutMetadata);
    if(ret == Ret::SUCCESS)
    {
        ret = m_layoutMetadataCodec.decode(etl::array_view<std::byte>{readBuffer.data(), retSize},
                                           retLayoutMetadata)
                  ? Ret::SUCCESS
                  : Ret::INVALID_DATA;
    }

    if(ret == Ret::SUCCESS && layout.has_value() && retLayoutMetadata.value().get().has_value())
    {
        layout.value().get().emplace(hash,
                                     retLayoutMetadata.value().get().value(),
                                     package.getNbImages(),
                                     m_fileSystem,
                                     inputPath);
    }

    return ret;
}

void LayoutStorage::getLayoutPath(uuid::UuidView hash, embedded_ostream& os) const
{
    os << m_directoryPath.data() << '/';
    os << hash;
    os << LAYOUT_EXTENSION;
}