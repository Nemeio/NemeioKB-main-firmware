#pragma once

#include "AbstractUserConfigurationWriter.hpp"
#include "UserConfiguration.hpp"
#include "fs.hpp"
#include "ISharedBufferContainer.hpp"
#include "IUserConfigurationCodec.hpp"
#include "LayoutConfigurationWriter.hpp"

class UserConfigurationWriter : public AbstractUserConfigurationWriter
{
public:
    UserConfigurationWriter(uuid::UuidView id,
                            fs::IFileSystem& fileSystem,
                            etl::string_view directoryPath,
                            IUserConfigurationCodec& userConfigurationCodec,
                            ILayoutConfigurationCodec& layoutConfigurationCodec,
                            ISharedBufferContainer& sharedBufferContainer);
    ~UserConfigurationWriter() final = default;

    configuration::Ret initWrite() final;
    configuration::Ret endWrite() final;

    configuration::Ret move(etl::string_view newDirectoryPath) final;

    IConfigurationFileWriter& createLayoutConfigurationWriter(uuid::UuidView id) final;

private:
    IUserConfigurationCodec& m_userConfigurationCodec;
    ILayoutConfigurationCodec& m_layoutConfigurationCodec;
    ISharedBufferContainer& m_sharedBufferContainer;
    etl::optional<LayoutConfigurationWriter> m_layoutConfigurationWriter;

    void getFileName(embedded_ostream& os) const override;
    configuration::Ret checkFile();
};