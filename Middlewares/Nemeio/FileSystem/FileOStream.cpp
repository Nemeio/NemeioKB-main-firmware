/*
 * FileOStream.cpp
 *
 *  Created on: 3 juil. 2020
 *      Author: eskoric
 */

#include "FileOStream.hpp"

FileOStream::FileOStream(fs::IFileSystem& fileSystem)
    : FileStream(fileSystem)
{
}

FileIOStreamErr FileOStream::write(const uint8_t* pData, std::size_t len, std::size_t& retWrittenLen)
{
    FileIOStreamErr ret = FileIOStreamErr::SUCCESS;

    if(!mbFileOpen)
    {
        return FileIOStreamErr::STATE;
    }

    retWrittenLen = 0;

    while(retWrittenLen != len)
    {
        if(ssize_t writeSz = mFile.write(pData, len); writeSz >= 0)
        {
            retWrittenLen += writeSz;
        }
        else
        {
            ret = FileIOStreamErr::FS;
            break;
        }
    }

    return ret;
}
