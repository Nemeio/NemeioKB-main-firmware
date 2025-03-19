/*
 * FileIOStream.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#ifndef FILESYSTEM_FILEIOSTREAM_FILEIOSTREAM_CPP_
#define FILESYSTEM_FILEIOSTREAM_FILEIOSTREAM_CPP_

#include "string.h"
#include "assertTools.h"
#include "FileStream.hpp"

FileStream::FileStream(fs::IFileSystem& fileSystem)
    : mFile(fileSystem)
{
}

FileStream::~FileStream()
{
    close();
}

FileIOStreamErr FileStream::open(std::string_view path, int flags)
{
    FileIOStreamErr ret = FileIOStreamErr::SUCCESS;

    if(mbFileOpen)
    {
        return FileIOStreamErr::STATE;
    }

    if(fs::FS_ERR_OK != mFile.open(path, flags))
    {
        ret = FileIOStreamErr::FS;
    }

    if(FileIOStreamErr::SUCCESS == ret)
    {
        mbFileOpen = true;
    }

    return ret;
}

FileIOStreamErr FileStream::close()
{
    FileIOStreamErr ret = FileIOStreamErr::SUCCESS;

    if(!mbFileOpen)
    {
        return FileIOStreamErr::STATE;
    }

    if(fs::FS_ERR_OK != mFile.close())
    {
        ASSERT(false);
    }
    else
    {
        mbFileOpen = false;
    }

    return ret;
}

FileIOStreamErr FileStream::seek(int32_t offset, std::size_t& filePos, fs::SeekFlag flag)
{
    FileIOStreamErr ret = FileIOStreamErr::SUCCESS;

    if(!mbFileOpen)
    {
        return FileIOStreamErr::STATE;
    }

    if(ssize_t retOffset = mFile.seek(offset, flag); retOffset < 0)
    {
        ret = FileIOStreamErr::FS;
    }
    else
    {
        filePos = static_cast<std::size_t>(retOffset);
    }

    return ret;
}

FileIOStreamErr FileStream::reset()
{
    std::size_t filePos = 0;
    return seek(0, filePos);
}

FileIOStreamErr FileStream::size(std::size_t& fileSize)
{
    FileIOStreamErr ret = FileIOStreamErr::SUCCESS;

    if(!mbFileOpen)
    {
        return FileIOStreamErr::STATE;
    }

    if(ssize_t size = mFile.size(); size > 0)
    {
        fileSize = size;
    }
    else
    {
        fileSize = 0;
        ret = FileIOStreamErr::FS;
    }

    return ret;
}

#endif /* FILESYSTEM_FILEIOSTREAM_FILEIOSTREAM_CPP_ */
