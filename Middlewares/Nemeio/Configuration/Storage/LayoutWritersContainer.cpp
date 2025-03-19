#include "LayoutWritersContainer.hpp"

LayoutWritersContainer::LayoutWritersContainer(fs::IFileSystem& fileSystem,
                                               ILayoutMetadataCodec& layoutMetadataCodec)
    : m_fileSystem(fileSystem)
    , m_layoutMetadataCodec(layoutMetadataCodec)
{
}

etl::optional<etl::reference_wrapper<AbstractConfigurationFileWriter>>
LayoutWritersContainer::createLayoutWriter(uuid::UuidView id,
                                           fs::IFileSystem& fileSystem,
                                           etl::string_view directoryPath)
{
    m_writer.emplace(id, fileSystem, directoryPath, m_layoutMetadataCodec);

    return etl::reference_wrapper<AbstractConfigurationFileWriter>(etl::ref(m_writer.value()));
}

void LayoutWritersContainer::resetLayoutWriter(uuid::UuidView id)
{
    if(m_writer.has_value() && m_writer.value().getId() == id)
    {
        m_writer.reset();
    }
}