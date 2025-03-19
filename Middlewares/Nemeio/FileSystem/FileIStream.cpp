/*
 * FileOStream.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#include "FileIStream.hpp"

FileIStream::FileIStream(fs::IFileSystem& fileSystem)
    : FileStream(fileSystem)
{
}

FileIOStreamErr FileIStream::read(uint8_t* pData, std::size_t len, std::size_t& retReadLen)
{
    FileIOStreamErr ret = FileIOStreamErr::SUCCESS;

    if(!mbFileOpen)
    {
        return FileIOStreamErr::STATE;
    }

    retReadLen = 0;


    if(ssize_t readSz = mFile.read(pData, len); readSz < 0)
    {
        ret = FileIOStreamErr::FS;
    }
    else
    {
        retReadLen = readSz;
    }

    return ret;
}
