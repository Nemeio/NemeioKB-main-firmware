/*
 * Dir.hpp
 *
 *  Created on: 26 janv. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FILESYSTEM_DIR_HPP_
#define NEMEIO_FILESYSTEM_DIR_HPP_

#include "IFileSystem.hpp"
#include "FileSystemCommon.hpp"

namespace fscommon
{
class Dir
{
public:
    Dir(IFileSystem& fileSystem);
    virtual ~Dir();

    int mkdir(const char* path);
    int open(std::string_view path);
    int close();
    int read(FSInfo& info);
    int seek(std::size_t off);
    ssize_t tell();
    int rewind();
    int findFirstFile(const std::string& path, const std::string& pattern, FSInfo& info);
    int findNextFile(const std::string& pattern, FSInfo& info);

private:
    DirHandle mDirHandle;
    IFileSystem& mFileSystem;
};

} /* namespace fscommon */

#endif /* NEMEIO_FILESYSTEM_DIR_HPP_ */
