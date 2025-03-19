#pragma once

#include "ILayoutStorage.hpp"
#include "ILayoutMetadataCodec.hpp"
#include "Mutex.hpp"
#include "LayoutListIterator.hpp"
#include "etl/pool.h"

class LayoutStorage : public ILayoutStorage
{
public:
    LayoutStorage(fs::IFileSystem& fileSystem,
                  etl::string_view directoryPath,
                  ILayoutMetadataCodec& layoutMetadataCodec);
    ~LayoutStorage() final = default;

    configuration::Ret deleteLayout(uuid::UuidView hash) final;
    void clear() final;
    configuration::Ret getLayout(uuid::UuidView hash,
                                 etl::optional<configuration::Layout>& layout) final;
    fs::IFileSystem& getFileSystem() final { return m_fileSystem; };

    etl::optional<etl::reference_wrapper<IUuidList::Iterator>> createIterator() final;
    void deleteIterator(IUuidList::Iterator& iterator) final;

private:
    static constexpr size_t MAX_ITERATORS = 2;

    fs::IFileSystem& m_fileSystem;
    etl::string<configuration::MAX_DIRECTORY_PATH_SIZE> m_directoryPath;
    ILayoutMetadataCodec& m_layoutMetadataCodec;

    Mutex m_poolMutex;
    etl::pool<LayoutListIterator, MAX_ITERATORS> m_iteratorPool;

    configuration::Ret readLayoutFromFile(etl::string_view inputPath,
                                          uuid::UuidView hash,
                                          OptionalReturn<configuration::Layout>& layout);

    void getLayoutPath(uuid::UuidView hash, embedded_ostream& os) const;
};