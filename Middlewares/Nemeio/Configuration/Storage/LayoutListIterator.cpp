
#include "LayoutListIterator.hpp"
#include "ConfigurationStorageUtil.hpp"

LayoutListIterator::LayoutListIterator(fs::IFileSystem& fileSystem, etl::string_view directoryPath)
    : m_dir(fileSystem)
{
    m_dir.open(directoryPath.data());
}

etl::optional<uuid::Uuid> LayoutListIterator::next()
{
    etl::optional<uuid::Uuid> uuid;
    int fsErr;
    do
    {
        fs::FSInfo info;

        fsErr = m_dir.read(info);

        if(info.type == fs::FileType::REG)
        {
            etl::string_view idStr{info.getName(), uuid::SIZE * 2};

            uuid = ConfigurationStorageUtil::strToId(idStr);
        }
    } while(fsErr > 0 && !uuid.has_value());

    return uuid;
}