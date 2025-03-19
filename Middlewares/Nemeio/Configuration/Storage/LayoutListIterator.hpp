#pragma once

#include "IUuidList.hpp"
#include "fs.hpp"

class LayoutListIterator : public IUuidList::Iterator
{
public:
    LayoutListIterator(fs::IFileSystem& fileSystem, etl::string_view directoryPath);
    ~LayoutListIterator() final = default;

    etl::optional<uuid::Uuid> next() final;

private:
    fs::Dir m_dir;
};