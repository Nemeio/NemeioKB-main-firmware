
#include "UserConfigurationListIterator.hpp"
#include "ConfigurationStorageUtil.hpp"

UserConfigurationListIterator::UserConfigurationListIterator(fs::IFileSystem& fileSystem,
                                                             etl::string_view directoryPath)
    : m_dir(fileSystem)
{
    m_dir.open(directoryPath.data());
}

etl::optional<uuid::Uuid> UserConfigurationListIterator::next()
{
    etl::optional<uuid::Uuid> uuid;
    int fsErr;

    do
    {
        fs::FSInfo info;

        fsErr = m_dir.read(info);

        if(etl::string_view entryName(info.getName());
           fsErr > 0 && info.type == fs::FileType::DIR && "." != entryName && ".." != entryName)
        {
            uuid = ConfigurationStorageUtil::strToId(entryName);
        }
    } while(fsErr > 0 && !uuid.has_value());

    return uuid;
}