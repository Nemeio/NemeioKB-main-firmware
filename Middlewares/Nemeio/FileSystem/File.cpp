/*
 * File.cpp
 *
 *  Created on: 26 janv. 2021
 *      Author: thomas
 */

#include "File.hpp"

namespace fscommon
{
File::File(IFileSystem& fileSystem)
    : mFileSystem(fileSystem)
{
    mFileHandle = INVALID_FILE_HANDLE;
}

File::File(IFileSystem& fileSystem, std::string_view path, int flags)
    : File(fileSystem)
{
    open(path, flags);
}


File::~File()
{
    if(mFileHandle != INVALID_FILE_HANDLE)
    {
        close();
    }
}

int File::open(std::string_view path, int flags)
{
    return mFileSystem.fileOpen(mFileHandle, path, flags);
}

int File::close()
{
    int error = mFileSystem.fileClose(mFileHandle);
    mFileHandle = INVALID_FILE_HANDLE;
    return error;
}

ssize_t File::read(void* buffer, std::size_t size)
{
    return mFileSystem.fileRead(mFileHandle, buffer, size);
}

ssize_t File::write(const void* buffer, std::size_t size)
{
    return mFileSystem.fileWrite(mFileHandle, buffer, size);
}

int File::sync()
{
    return mFileSystem.fileSync(mFileHandle);
}

int File::remove(const std::string_view& path)
{
    return mFileSystem.remove(path);
}

int File::rename(std::string_view oldpath, std::string_view newpath)
{
    return mFileSystem.rename(oldpath, newpath);
}

ssize_t File::seek(ssize_t off, SeekFlag flag)
{
    return mFileSystem.fileSeek(mFileHandle, off, flag);
}

int File::truncate(std::size_t size)
{
    return mFileSystem.fileTruncate(mFileHandle, size);
}

ssize_t File::tell()
{
    return mFileSystem.fileTell(mFileHandle);
}

int File::rewind()
{
    return mFileSystem.fileRewind(mFileHandle);
}

ssize_t File::size()
{
    return mFileSystem.fileSize(mFileHandle);
}

} /* namespace fscommon */
