/*
 * FileIOStream.hpp
 *
 *  Created on: Jan 7, 2021
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_FILEIOSTREAM_HPP_
#define NEMEIO_FILESYSTEM_FILEIOSTREAM_HPP_

#include "FileIStream.hpp"
#include "FileOStream.hpp"

class FileIOStream : public FileIStream, public FileOStream
{
public:
    explicit FileIOStream(fs::IFileSystem& fileSystem);
    virtual ~FileIOStream() = default;
};

#endif /* NEMEIO_FILESYSTEM_FILEIOSTREAM_HPP_ */
