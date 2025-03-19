/*
 * FileIOStream.hpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_FILESYSTEM_FILESTREAM_HPP_
#define NEMEIO_FILESYSTEM_FILESTREAM_HPP_

#include <string>
#include "fs.hpp"
#include "IFileSystem.hpp"

enum class FileIOStreamErr
{
    SUCCESS,
    STATE,
    FS
};

class FileStream
{
public:
    explicit FileStream(fs::IFileSystem& fileSystem);
    virtual ~FileStream();
    FileStream(FileStream&) = delete;
    FileStream& operator=(const FileStream&) = delete;
    FileStream(FileStream&&) = delete;
    FileStream const& operator=(FileStream&&) = delete;


    FileIOStreamErr open(std::string_view path, int flags);
    FileIOStreamErr close();
    FileIOStreamErr seek(int32_t offset,
                         std::size_t& filePos,
                         fs::SeekFlag flag = fs::SeekFlag::ABSOLUTE_POS);
    FileIOStreamErr reset();
    FileIOStreamErr size(std::size_t& fileSize);

protected:
    fs::File mFile;
    bool mbFileOpen = false;
};

#endif /* NEMEIO_FILESYSTEM_FILESTREAM_HPP_ */
