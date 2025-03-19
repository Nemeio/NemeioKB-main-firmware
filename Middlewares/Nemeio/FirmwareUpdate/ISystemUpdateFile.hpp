/*
 * ISystemUpdateFile.hpp
 *
 *  Created on: 20 sept. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_ISYSTEMUPDATEFILE_HPP_
#define NEMEIO_FIRMWAREUPDATE_ISYSTEMUPDATEFILE_HPP_

#include "fs.hpp"

class ISystemUpdateFile
{
public:
    ISystemUpdateFile() = default;
    virtual ~ISystemUpdateFile() = default;

    virtual fs::FSErrCode open() = 0;
    virtual fs::FSErrCode write(const uint8_t* pData, std::size_t len) = 0;
    virtual fs::FSErrCode close() = 0;

    virtual fs::IFileSystem& getFileSystem() const = 0;
    virtual std::string_view getFilePath() const = 0;
};


#endif /* NEMEIO_FIRMWAREUPDATE_ISYSTEMUPDATEFILE_HPP_ */
