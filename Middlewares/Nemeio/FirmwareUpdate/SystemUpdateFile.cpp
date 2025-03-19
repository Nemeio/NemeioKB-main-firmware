/*
 * SystemUpdateFile.cpp
 *
 *  Created on: 20 sept. 2021
 *      Author: thomas
 */

#include "SystemUpdateFile.hpp"

SystemUpdateFile::SystemUpdateFile(fs::IFileSystem& updateFileSystem,
                                   std::string_view updateFilePath)
    : mSystemPackagePath(updateFilePath)
    , mFileSystem(updateFileSystem)
    , mPackageFile(mFileSystem)
{
}

fs::FSErrCode SystemUpdateFile::open()
{
    fs::FSErrCode err = fs::FSErrCode::FS_ERR_IO;
    mPackageFile.close(); /* Close if not correctly closed before */
    if(mPackageFile.remove(mSystemPackagePath) == fs::FS_ERR_OK
       && mPackageFile.open(mSystemPackagePath, fs::OpenFlag::CREAT) == fs::FS_ERR_OK)
    { /* Can't open again if write or runUpdate fails I think  (no "close" called) */
        err = fs::FSErrCode::FS_ERR_OK;
    }
    return err;
}

fs::FSErrCode SystemUpdateFile::write(const uint8_t* pData, std::size_t len)
{
    fs::FSErrCode err = fs::FSErrCode::FS_ERR_CORRUPT;
    if(mPackageFile.write(pData, len) == len)
    {
        err = fs::FSErrCode::FS_ERR_OK;
    }
    return err;
}

fs::FSErrCode SystemUpdateFile::close()
{
    /* Flush the content not flushed in flash by closing the file */
    fs::FSErrCode ret = fs::FSErrCode::FS_ERR_OK;

    if(mPackageFile.close() != fs::FS_ERR_OK)
    {
        ret = fs::FSErrCode::FS_ERR_IO;
    }

    return ret;
}

fs::IFileSystem& SystemUpdateFile::getFileSystem() const
{
    return mFileSystem;
}

std::string_view SystemUpdateFile::getFilePath() const
{
    return mSystemPackagePath;
}
