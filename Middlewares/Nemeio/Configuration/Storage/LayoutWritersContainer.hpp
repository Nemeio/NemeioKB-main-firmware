#pragma once

#include "ILayoutWritersContainer.hpp"
#include "LayoutWriter.hpp"
#include "fs.hpp"

class LayoutWritersContainer : public ILayoutWritersContainer
{
public:
    explicit LayoutWritersContainer(fs::IFileSystem& fileSystem,
                                    ILayoutMetadataCodec& layoutMetadataCodec);
    ~LayoutWritersContainer() final = default;

    etl::optional<etl::reference_wrapper<AbstractConfigurationFileWriter>> createLayoutWriter(
        uuid::UuidView id, fs::IFileSystem& fileSystem, etl::string_view directoryPath) final;

    void resetLayoutWriter(uuid::UuidView id) final;

private:
    fs::IFileSystem& m_fileSystem;
    ILayoutMetadataCodec& m_layoutMetadataCodec;
    etl::optional<LayoutWriter> m_writer;
};