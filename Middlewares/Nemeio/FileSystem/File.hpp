/*
 * File.hpp
 *
 *  Created on: 26 janv. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FILESYSTEM_FILE_HPP_
#define NEMEIO_FILESYSTEM_FILE_HPP_

#include "IFileSystem.hpp"
#include "FileSystemCommon.hpp"
#include "cpp_helper.hpp"

namespace fscommon
{
class File
{
public:
    File(IFileSystem& fileSystem);
    File(IFileSystem& fileSystem, std::string_view path, int flags);
    virtual ~File();

    COPYABLE(File, delete);
    MOVABLE(File, default);

    int open(std::string_view path, int flags);
    int close();
    ssize_t read(void* buffer, std::size_t size);
    ssize_t write(const void* buffer, std::size_t size);
    int sync();
    int remove(const std::string_view& path);
    int rename(std::string_view oldpath, std::string_view newpath);
    ssize_t seek(ssize_t off, SeekFlag flag);
    int truncate(std::size_t size);
    ssize_t tell();
    int rewind();
    ssize_t size();

private:
    IFileSystem& mFileSystem;
    FileHandle mFileHandle;
};

} /* namespace fscommon */

#endif /* NEMEIO_FILESYSTEM_FILE_HPP_ */
