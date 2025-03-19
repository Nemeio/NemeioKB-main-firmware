#pragma once

#include "IConfigurationFileWriter.hpp"
#include "fs.hpp"
#include "ILayoutConfigurationCodec.hpp"
#include "ConfigurationCommon.hpp"

class AbstractConfigurationFileWriter : public IConfigurationFileWriter
{
public:
    AbstractConfigurationFileWriter(uuid::UuidView id,
                                    fs::IFileSystem& fileSystem,
                                    etl::string_view directoryPath);
    ~AbstractConfigurationFileWriter() override = default;

    configuration::Ret initWrite() override;
    configuration::Ret write(etl::array_view<const std::byte> data) override;
    virtual configuration::Ret endWrite() = 0;
    configuration::Ret move(etl::string_view newDirectoryPath) override;

    uuid::UuidView getId() const final { return uuid::UuidView{m_id}; }

protected:
    uuid::Uuid m_id;
    fs::IFileSystem& m_fileSystem;
    etl::string<configuration::MAX_DIRECTORY_PATH_SIZE> m_directoryPath;
    fs::File m_file;

    void getFilePath(embedded_ostream& os) const;
    virtual void getFileName(embedded_ostream& os) const = 0;
};