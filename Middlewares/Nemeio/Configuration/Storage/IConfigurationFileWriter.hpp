#pragma once

#include "Uuid.hpp"
#include "ConfigurationCommon.hpp"
#include "etl/string_view.h"

class IConfigurationFileWriter
{
public:
    IConfigurationFileWriter() = default;
    virtual ~IConfigurationFileWriter() = default;

    virtual configuration::Ret initWrite() = 0;
    virtual configuration::Ret write(etl::array_view<const std::byte> data) = 0;
    virtual configuration::Ret endWrite() = 0;
    virtual configuration::Ret move(etl::string_view newDirectoryPath) = 0;

    virtual uuid::UuidView getId() const = 0;
};