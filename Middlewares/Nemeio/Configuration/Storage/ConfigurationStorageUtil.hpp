#pragma once

#include "fs.hpp"
#include "ConfigurationCommon.hpp"
#include "etl/string_view.h"
#include "etl/optional.h"
#include "Uuid.hpp"
#include "embedded_ostream.hpp"

class ConfigurationStorageUtil
{
public:
    static configuration::Ret renameBackupExisting(fs::IFileSystem& fs,
                                                   etl::string_view oldPath,
                                                   etl::string_view newPath);
    static configuration::Ret readFile(fs::IFileSystem& fs,
                                       etl::string_view inputPath,
                                       etl::array_view<std::byte> buffer,
                                       size_t& retSize);
    static configuration::Ret convertFsErrorCode(int fsErr);
    static etl::optional<uuid::Uuid> strToId(etl::string_view str);
    static bool isExistingPath(fs::IFileSystem& fs, etl::string_view path);

private:
    static constexpr char BACKUP_FOOTER[] = "_bkp";
};