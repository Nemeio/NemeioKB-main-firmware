#pragma once

#include "IUserConfigurationWritersContainer.hpp"
#include "UserConfigurationWriter.hpp"
#include "fs.hpp"

class UserConfigurationWritersContainer : public IUserConfigurationWritersContainer
{
public:
    UserConfigurationWritersContainer(fs::IFileSystem& fileSystem,
                                      IUserConfigurationCodec& userConfigurationCodec,
                                      ILayoutConfigurationCodec& layoutConfigurationCodec,
                                      ISharedBufferContainer& sharedBufferContainer);
    ~UserConfigurationWritersContainer() final = default;

    etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>>
    createUserConfigurationWriter(uuid::UuidView id,
                                  fs::IFileSystem& fileSystem,
                                  etl::string_view directoryPath) final;

    etl::optional<etl::reference_wrapper<AbstractUserConfigurationWriter>> getUserConfigurationWriter(
        uuid::UuidView id) final;

    void resetUserConfigurationWriter(uuid::UuidView id) final;

private:
    fs::IFileSystem& m_fileSystem;
    IUserConfigurationCodec& m_userConfigurationCodec;
    ILayoutConfigurationCodec& m_layoutConfigurationCodec;
    ISharedBufferContainer& m_sharedBufferContainer;
    etl::optional<UserConfigurationWriter> m_writer;
};