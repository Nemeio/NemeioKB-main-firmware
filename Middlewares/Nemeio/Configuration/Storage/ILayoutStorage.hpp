#pragma once

#include "fs.hpp"
#include "Layout.hpp"
#include "ConfigurationCommon.hpp"
#include "etl/string_view.h"
#include "etl/vector.h"
#include "IUuidList.hpp"

class ILayoutStorage : public IUuidList
{
public:
    ILayoutStorage() = default;
    ~ILayoutStorage() override = default;

    virtual configuration::Ret deleteLayout(uuid::UuidView hash) = 0;
    virtual void clear() = 0;
    virtual configuration::Ret getLayout(uuid::UuidView hash,
                                         etl::optional<configuration::Layout>& layout) = 0;
    virtual fs::IFileSystem& getFileSystem() = 0;
};