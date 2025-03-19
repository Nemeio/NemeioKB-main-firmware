/*
 * Dir.cpp
 *
 *  Created on: 26 janv. 2021
 *      Author: thomas
 */

#include "Dir.hpp"

namespace fscommon
{
Dir::Dir(IFileSystem& fileSystem)
    : mFileSystem(fileSystem)
{
    mDirHandle = INVALID_DIR_HANDLE;
}

Dir::~Dir()
{
    if(mDirHandle != INVALID_DIR_HANDLE)
    {
        close();
    }
}

int Dir::mkdir(const char* path)
{
    return mFileSystem.mkdir(path);
}


int Dir::open(std::string_view path)
{
    return mFileSystem.dirOpen(mDirHandle, path);
}

int Dir::close()
{
    int ret = mFileSystem.dirClose(mDirHandle);
    mDirHandle = INVALID_DIR_HANDLE;
    return ret;
}

int Dir::read(FSInfo& info)
{
    return mFileSystem.dirRead(mDirHandle, info);
}

int Dir::seek(size_t off)
{
    return mFileSystem.dirSeek(mDirHandle, off);
}

ssize_t Dir::tell()
{
    return mFileSystem.dirTell(mDirHandle);
}

int Dir::rewind()
{
    return mFileSystem.dirRewind(mDirHandle);
}

int Dir::findFirstFile(const std::string& path, const std::string& pattern, FSInfo& info)
{
    return mFileSystem.findFirstFile(mDirHandle, path, pattern, info);
}

int Dir::findNextFile(const std::string& pattern, FSInfo& info)
{
    return mFileSystem.findNextFile(mDirHandle, pattern, info);
}

} /* namespace fscommon */
