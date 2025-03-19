#include "UserConfigurationWritersContainer.hpp"

UserConfigurationWritersContainer::UserConfigurationWritersContainer(
    fs::IFileSystem& fileSystem,
    IUserConfigurationCodec& userConfigurationCodec,
    ILayoutConfigurationCodec& layoutConfigurationCodec,
    ISharedBufferContainer& sharedBufferContainer)
    : m_fileSystem(fileSystem)
    , m_userConfigurationCodec(userConfigurationCodec)
    , m_layoutConfigurationCodec(layoutConfigurationCodec)
    , m_sharedBufferContainer(sharedBufferContainer)
{
}

etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>>
UserConfigurationWritersContainer::createUserConfigurationWriter(uuid::UuidView id,
                                                                 fs::IFileSystem& fileSystem,
                                                                 etl::string_view directoryPath)
{
    m_writer.emplace(id,
                     fileSystem,
                     directoryPath,
                     m_userConfigurationCodec,
                     m_layoutConfigurationCodec,
                     m_sharedBufferContainer);

    return etl::reference_wrapper<AbstractUserConfigurationWriter>(etl::ref(m_writer.value()));
}

etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>>
UserConfigurationWritersContainer::getUserConfigurationWriter(uuid::UuidView id)
{
    return m_writer.has_value() && m_writer.value().getId() == id ? etl::optional(
               etl::reference_wrapper<AbstractUserConfigurationWriter>(etl::ref(m_writer.value())))
                                                                  : etl::nullopt;
}

void UserConfigurationWritersContainer::resetUserConfigurationWriter(uuid::UuidView id)
{
    if(m_writer.has_value() && m_writer.value().getId() == id)
    {
        m_writer.reset();
    }
}