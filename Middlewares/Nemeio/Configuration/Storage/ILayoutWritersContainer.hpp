#pragma once

#include "AbstractConfigurationFileWriter.hpp"
#include "etl/optional.h"
#include "fs.hpp"

class ILayoutWritersContainer
{
public:
    ILayoutWritersContainer() = default;
    virtual ~ILayoutWritersContainer() = default;

    virtual etl::optional<etl::reference_wrapper<AbstractConfigurationFileWriter>> createLayoutWriter(
        uuid::UuidView id, fs::IFileSystem& fileSystem, etl::string_view directoryPath) = 0;

    virtual void resetLayoutWriter(uuid::UuidView id) = 0;
};