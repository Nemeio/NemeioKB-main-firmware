/*
 * ReceiveDataFileListener.cpp
 *
 *  Created on: Jun 10, 2019
 *      Author: Labo
 */

#include <ReceiveData/ReceiveDataListeners/ReceiveDataFileListener.hpp>

ReceiveDataFileListener::ReceiveDataFileListener(AppCommDefs::ReceiveDataID dataId,
                                                 fs::IFileSystem& fileSystem,
                                                 std::string_view path)
    : ReceiveDataListener(dataId)
    , mPath(etl::string_view(path.data()))
    , mFileSystem(fileSystem)
    , mFileStream(fileSystem)
{
}

uint8_t ReceiveDataFileListener::open(std::size_t& retDataLen)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(mFileStream.open(mPath.c_str(), fs::OpenFlag::RDONLY) != FileIOStreamErr::SUCCESS
       || mFileStream.size(retDataLen) != FileIOStreamErr::SUCCESS)
    {
        ret = AppCommDefs::APPCOMMERR_FS;
    }

    return ret;
}

uint8_t ReceiveDataFileListener::doPrepareReceive(const uint8_t*, size_t, std::size_t& retDataLen)
{
    return open(retDataLen);
}

uint8_t ReceiveDataFileListener::doReceiveData(uint8_t* pData,
                                               size_t len,
                                               std::size_t& retReceiveLength)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(mFileStream.read(pData, len, retReceiveLength) != FileIOStreamErr::SUCCESS)
    {
        ret = AppCommDefs::APPCOMMERR_FS;
    }

    return ret;
}

uint8_t ReceiveDataFileListener::doTerminateReceive()
{
    uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;

    if(mFileStream.close() != FileIOStreamErr::SUCCESS)
    {
        ret = AppCommDefs::APPCOMMERR_FS;
    }

    mFileStream.close();

    doReceiveFileDone();

    return ret;
}

void ReceiveDataFileListener::doReset()
{
    mFileStream.close();
}

void ReceiveDataFileListener::doReceiveFileDone()
{
}
