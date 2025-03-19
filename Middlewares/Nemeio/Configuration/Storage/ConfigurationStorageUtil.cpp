#include "ConfigurationStorageUtil.hpp"
#include "etl/array_view.h"
#include "etl/vector.h"
#include "etl/string.h"

using namespace configuration;

Ret ConfigurationStorageUtil::renameBackupExisting(fs::IFileSystem& fs,
                                                   etl::string_view oldPath,
                                                   etl::string_view newPath)
{
    embedded_ostream pathBackup;
    pathBackup << newPath << BACKUP_FOOTER;

    fs.remove_recursive(pathBackup.str().c_str());

    int fsErr = fs.rename(newPath.data(), pathBackup.str().c_str());

    if(fsErr == fs::FS_ERR_NOENT)
    {
        // There is no data at newPath to backup
        fsErr = fs::FS_ERR_OK;
    }

    if(fsErr == fs::FS_ERR_OK)
    {
        fsErr = fs.rename(oldPath.data(), newPath.data());
    }

    if(fsErr == fs::FS_ERR_OK)
    {
        fs.remove_recursive(pathBackup.str().c_str());
    }
    else
    {
        fs.rename(pathBackup.str().c_str(), newPath.data());
    }

    return convertFsErrorCode(static_cast<fs::FSErrCode>(fsErr));
}

configuration::Ret ConfigurationStorageUtil::readFile(fs::IFileSystem& fs,
                                                      etl::string_view inputPath,
                                                      etl::array_view<std::byte> buffer,
                                                      size_t& retSize)
{
    fs::File file(fs);
    int fsErr = file.open(inputPath.data(), fs::OpenFlag::RDONLY);
    Ret ret = convertFsErrorCode(static_cast<fs::FSErrCode>(fsErr));

    if(ret == Ret::SUCCESS)
    {
        size_t size = file.size();
        ssize_t readBytes = file.read(buffer.data(), buffer.size());

        file.close();

        if(readBytes == size)
        {
            retSize = size;
            ret = Ret::SUCCESS;
        }
        else
        {
            ret = convertFsErrorCode(static_cast<fs::FSErrCode>(readBytes));
        }
    }

    return ret;
}

Ret ConfigurationStorageUtil::convertFsErrorCode(int fsErr)
{
    Ret ret;

    switch(static_cast<fs::FSErrCode>(fsErr))
    {
    case fs::FS_ERR_OK:
        ret = Ret::SUCCESS;
        break;
    case fs::FS_ERR_NOENT:
        ret = Ret::NOT_FOUND;
        break;
    case fs::FS_ERR_NOSPC:
        ret = Ret::NO_SPACE;
        break;
    default:
        ret = Ret::FS;
        break;
    }

    return ret;
}

etl::optional<uuid::Uuid> ConfigurationStorageUtil::strToId(etl::string_view str)
{
    static constexpr size_t BYTE_HEX_STR_SIZE = 2;

    etl::optional<uuid::Uuid> ret{etl::nullopt};

    etl::vector<std::byte, uuid::SIZE> bytes;

    if(str.size() != uuid::SIZE * BYTE_HEX_STR_SIZE)
    {
        return ret;
    }

    bool success = true;

    for(size_t i = 0; i < str.size(); i += BYTE_HEX_STR_SIZE)
    {
        etl::string<BYTE_HEX_STR_SIZE> byteStr(str.substr(i, BYTE_HEX_STR_SIZE));
        char* end = nullptr;
        auto byte = static_cast<std::byte>(strtoul(byteStr.data(), &end, 16));
        if(end == byteStr.data())
        {
            success = false;
            break;
        }
        bytes.push_back(byte);
    }

    if(success)
    {
        ret.emplace(etl::array_view{bytes});
    }

    return ret;
}

bool ConfigurationStorageUtil::isExistingPath(fs::IFileSystem& fs, etl::string_view path)
{
    fs::FSInfo info;
    int fsErr = fs.stat(path.data(), info);

    return fsErr == fs::FSErrCode::FS_ERR_OK;
}