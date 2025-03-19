#pragma once

#include "AbstractConfigurationFileWriter.hpp"
#include "fs.hpp"
#include "ILayoutMetadataCodec.hpp"
#include "ConfigurationCommon.hpp"

class LayoutWriter : public AbstractConfigurationFileWriter
{
public:
    LayoutWriter(uuid::UuidView id,
                 fs::IFileSystem& fileSystem,
                 etl::string_view directoryPath,
                 ILayoutMetadataCodec& layoutMetadataCodec);
    ~LayoutWriter() final = default;

    configuration::Ret endWrite() final;
    configuration::Ret move(etl::string_view newDirectoryPath) final;

private:
    ILayoutMetadataCodec& m_layoutMetadataCodec;

    configuration::Ret checkFile();
    void getFileName(embedded_ostream& os) const;
};