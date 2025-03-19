/*
 * SendDataFileListener.cpp
 *
 *  Created on: Feb 5, 2019
 *      Author: Labo
 */

#include <string.h>
#include "assertTools.h"
#include "SendDataFileListener.hpp"

SendDataFileListener::SendDataFileListener(AppCommDefs::SendDataID dataId,
                                           fs::IFileSystem& fileSystem,
                                           std::string_view path)
    : SendDataListener(dataId)
    , mPath(path)
    , mFileStream(fileSystem)
    , mFileSystem(fileSystem)
{
}

SendDataFileListener::~SendDataFileListener()
{
    close();
}

uint8_t SendDataFileListener::doPrepareSend(const uint8_t*, size_t, size_t)
{
    return open();
}

uint8_t SendDataFileListener::doSendData(const uint8_t* pData, size_t len)
{
    return write(pData, len);
}

uint8_t SendDataFileListener::doTerminateSend()
{
    return close();
}

uint8_t SendDataFileListener::open()
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(mbFileOpen)
    {
        return AppCommDefs::APPCOMMERR_STATE;
    }

    if(FileIOStreamErr::SUCCESS
       != mFileStream.open(mPath, fs::OpenFlag::CREAT | fs::OpenFlag::RDWR | fs::OpenFlag::TRUNC))
    {
        ret = AppCommDefs::APPCOMMERR_FS;
    }

    if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
    {
        mbFileOpen = true;
    }

    return ret;
}

uint8_t SendDataFileListener::write(const uint8_t* pData, size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    ASSERT(pData != NULL);

    if(!mbFileOpen)
    {
        return AppCommDefs::APPCOMMERR_STATE;
    }

    if(std::size_t szDataWritten = 0;
       FileIOStreamErr::SUCCESS != mFileStream.write(pData, len, szDataWritten)
       || szDataWritten != len)
    {
        closeAndDelete();
        ret = AppCommDefs::APPCOMMERR_FS;
    }

    return ret;
}

uint8_t SendDataFileListener::close()
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(mbFileOpen)
    {
        if(FileIOStreamErr::SUCCESS != mFileStream.close())
        {
            ret = AppCommDefs::APPCOMMERR_FS;
        }
        else
        {
            mbFileOpen = false;
        }
    }

    return ret;
}

void SendDataFileListener::doReset()
{
    close();
}

void SendDataFileListener::closeAndDelete()
{
    close();
    mFileSystem.remove(mPath);
}

fs::IFileSystem& SendDataFileListener::getFileSystem()
{
    return mFileSystem;
}

std::string_view SendDataFileListener::getPath() const
{
    return mPath;
}

FileIStream& SendDataFileListener::getFileStreamRead()
{
    return mFileStream;
}
