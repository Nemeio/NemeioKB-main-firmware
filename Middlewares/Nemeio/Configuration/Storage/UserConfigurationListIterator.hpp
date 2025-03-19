#pragma once

#include "IUuidList.hpp"
#include "fs.hpp"

class UserConfigurationListIterator : public IUuidList::Iterator
{
public:
    UserConfigurationListIterator(fs::IFileSystem& fileSystem, etl::string_view directoryPath);
    ~UserConfigurationListIterator() final = default;

    etl::optional<uuid::Uuid> next() final;

private:
    fs::Dir m_dir;
};