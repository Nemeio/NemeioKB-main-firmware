/*
 * FileOStream.hpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_FILESYSTEM_FILEISTREAM_HPP_
#define NEMEIO_FILESYSTEM_FILEISTREAM_HPP_

#include "FileStream.hpp"

class FileIStream : public virtual FileStream
{
public:
    explicit FileIStream(fs::IFileSystem& fileSystem);
    virtual ~FileIStream() = default;

    FileIOStreamErr read(uint8_t* pData, std::size_t len, std::size_t& retReadLen);
};

#endif /* NEMEIO_FILESYSTEM_FILEISTREAM_HPP_ */
