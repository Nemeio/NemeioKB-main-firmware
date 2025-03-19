#pragma once

#include "AbstractConfigurationFileWriter.hpp"
#include "etl/string_view.h"

class AbstractUserConfigurationWriter : public AbstractConfigurationFileWriter
{
public:
    AbstractUserConfigurationWriter(uuid::UuidView id,
                                    fs::IFileSystem& fileSystem,
                                    etl::string_view directoryPath)
        : AbstractConfigurationFileWriter(id, fileSystem, directoryPath)
    {
    }
    ~AbstractUserConfigurationWriter() override = default;

    virtual IConfigurationFileWriter& createLayoutConfigurationWriter(uuid::UuidView id) = 0;
};