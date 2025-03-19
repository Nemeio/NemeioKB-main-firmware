/*
 * FileOStream.hpp
 *
 *  Created on: 3 juil. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_FILEOSTREAM_HPP_
#define NEMEIO_FILESYSTEM_FILEOSTREAM_HPP_

#include "FileStream.hpp"

class FileOStream : public virtual FileStream
{
public:
    explicit FileOStream(fs::IFileSystem& fileSystem);
    virtual ~FileOStream() = default;

    FileIOStreamErr write(const uint8_t* pData, std::size_t len, std::size_t& retWrittenLen);
};

#endif /* NEMEIO_FILESYSTEM_FILEOSTREAM_HPP_ */
