#pragma once

#include "AbstractConfigurationFileWriter.hpp"
#include "fs.hpp"
#include "ISharedBufferContainer.hpp"
#include "ILayoutConfigurationCodec.hpp"
#include "ConfigurationCommon.hpp"

class LayoutConfigurationWriter : public AbstractConfigurationFileWriter
{
public:
    LayoutConfigurationWriter(uuid::UuidView id,
                              fs::IFileSystem& fileSystem,
                              etl::string_view directoryPath,
                              ILayoutConfigurationCodec& layoutConfigurationCodec,
                              ISharedBufferContainer& sharedBufferContainer);
    ~LayoutConfigurationWriter() final = default;

    configuration::Ret endWrite() final;

private:
    ILayoutConfigurationCodec& m_layoutConfigurationCodec;
    ISharedBufferContainer& m_sharedBufferContainer;

    configuration::Ret checkFile();
    void getFileName(embedded_ostream& os) const;
};