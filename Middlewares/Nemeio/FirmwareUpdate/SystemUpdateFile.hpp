/*
 * SystemUpdateFile.hpp
 *
 *  Created on: 20 sept. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_SYSTEMUPDATEFILE_HPP_
#define NEMEIO_FIRMWAREUPDATE_SYSTEMUPDATEFILE_HPP_

#include <string_view>
#include "fs.hpp"
#include "ISystemUpdateFile.hpp"
#include "cpp_helper.hpp"

class SystemUpdateFile : public ISystemUpdateFile
{
public:
    SystemUpdateFile() = delete;
    explicit SystemUpdateFile(fs::IFileSystem& updateFileSystem, std::string_view updateFilePath);
    virtual ~SystemUpdateFile() = default;

    COPYABLE_MOVABLE(SystemUpdateFile, delete);

    fs::FSErrCode open() final;
    fs::FSErrCode write(const uint8_t* pData, std::size_t len) final;
    fs::FSErrCode close() final;

    fs::IFileSystem& getFileSystem() const final;
    std::string_view getFilePath() const final;

private:
    std::string_view mSystemPackagePath;
    fs::IFileSystem& mFileSystem;
    fs::File mPackageFile;
};

#endif /* NEMEIO_FIRMWAREUPDATE_SYSTEMUPDATEFILE_HPP_ */
