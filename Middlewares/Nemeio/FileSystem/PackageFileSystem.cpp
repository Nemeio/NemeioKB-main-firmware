/*
 * PackageFileSystem.cpp
 *
 *  Created on: 28 janv. 2021
 *      Author: thomas
 */

#include <PackageFileSystem.hpp>
#include <cassert>

using namespace fscommon;

PackageFileSystem::PackageFileSystem(fs::IFileSystem& fileSystem, std::string_view path)
    : mFirmwarePackageFile(fileSystem, path, fs::OpenFlag::RDONLY)
    , mFirmwarePackageFilePath(path.data())
{
    handle emptyHandle = {0};
    mHandles.fill(emptyHandle);
    memset(&mFirmwarePackageMetadata, 0, sizeof(mFirmwarePackageMetadata));
}

bool PackageFileSystem::getVersion(const std::string_view path, packageVersion& version)
{
    bool ret = false;

    if(!isMounted())
    {
        return false;
    }

    eFirmwareFileIndex index = getFirmwareIndex(path);
    if(index != FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        memcpy(&version, &(mFirmwarePackageMetadata.firmwares[index].version), sizeof(version));
        ret = true;
    }
    else
    {
        memset(&version, 0, sizeof(version));
    }
    return ret;
}

uint8_t PackageFileSystem::getFilesNumber() const
{
    return mFirmwarePackageMetadata.global.latest.firmwareNumber;
}

int PackageFileSystem::init()
{
    return FS_ERR_OK;
}

int PackageFileSystem::deinit()
{
    return FS_ERR_OK;
}

int PackageFileSystem::format()
{
    return mFirmwarePackageFile.remove(mFirmwarePackageFilePath.c_str());
}

int PackageFileSystem::loadFirmwaresInformation()
{
    int err = FS_ERR_OK;
    size_t lastFirmwareOffset = 0;

    for(uint8_t firmware = 0; firmware < mFirmwarePackageMetadata.global.latest.firmwareNumber;
        ++firmware)
    {
        if(mFirmwarePackageFile.read(&(mFirmwarePackageMetadata.firmwares[firmware]),
                                     sizeof(mFirmwarePackageMetadata.firmwares[firmware]))
           != sizeof(mFirmwarePackageMetadata.firmwares[firmware]))
        {
            err = FS_ERR_CORRUPT;
            break;
        }

        if(mFirmwarePackageMetadata.firmwares[firmware].firmware_offset < lastFirmwareOffset
           || mFirmwarePackageMetadata.firmwares[firmware].firmware_offset
                      + mFirmwarePackageMetadata.firmwares[firmware].firmware_size
                  < lastFirmwareOffset
           || mFirmwarePackageMetadata.firmwares[firmware].firmware_offset
                      + mFirmwarePackageMetadata.firmwares[firmware].firmware_size
                  > mFirmwarePackageMetadata.global.common.size)
        {
            err = FS_ERR_CORRUPT;
            break;
        }

        lastFirmwareOffset = mFirmwarePackageMetadata.firmwares[firmware].firmware_offset
                             + mFirmwarePackageMetadata.firmwares[firmware].firmware_size;
    }

    return err;
}

int PackageFileSystem::mount()
{
    AutoLock autoLock(mMutex);
    int err = FS_ERR_OK;

    s_FirmwarePackageMetadata tempMetadata;

    if(mFirmwarePackageFile.read(&(tempMetadata.global.common), sizeof(tempMetadata.global.common))
       != sizeof(tempMetadata.global.common))
    {
        err = FS_ERR_CORRUPT;
    }

    /* A null signature is invalid with ECDSA so it ok to set -the signature to 0 for versions < 3 */
    switch(tempMetadata.global.common.version)
    {
    case 1:
        if(mFirmwarePackageFile.read(&(tempMetadata.global.v1), sizeof(tempMetadata.global.v1))
           != sizeof(tempMetadata.global.v1))
        {
            err = FS_ERR_CORRUPT;
        }
        memcpy(&mFirmwarePackageMetadata, &tempMetadata, sizeof(s_FirmwarePackageMetadata));

        memset(mFirmwarePackageMetadata.global.latest.signature,
               0,
               sizeof(mFirmwarePackageMetadata.global.latest.signature));
        mFirmwarePackageMetadata.global.latest.firmwareNumber =
            3; /* Only support firmware package with 3 firmwares */

        mFirmwarePackageMetadata.global.common.version = LATEST_VERSION;
        break;
    case 2:
        if(mFirmwarePackageFile.read(&(tempMetadata.global.v2), sizeof(tempMetadata.global.v2))
           != sizeof(tempMetadata.global.v2))
        {
            err = FS_ERR_CORRUPT;
        }
        memcpy(&mFirmwarePackageMetadata, &tempMetadata, sizeof(s_FirmwarePackageMetadata));

        memset(mFirmwarePackageMetadata.global.latest.signature,
               0,
               sizeof(mFirmwarePackageMetadata.global.latest.signature));
        mFirmwarePackageMetadata.global.latest.firmwareNumber = tempMetadata.global.v2.firmwareNumber;

        mFirmwarePackageMetadata.global.common.version = LATEST_VERSION;
        break;
    case 3:
        if(mFirmwarePackageFile.read(&(tempMetadata.global.v3), sizeof(tempMetadata.global.v3))
           != sizeof(tempMetadata.global.v3))
        {
            err = FS_ERR_CORRUPT;
        }
        memcpy(&mFirmwarePackageMetadata, &tempMetadata, sizeof(s_FirmwarePackageMetadata));
        break;
    default:
        err = FS_ERR_CORRUPT;
        break;
    }

    if(err == FS_ERR_OK && mFirmwarePackageMetadata.global.common.magicNumber != HEADER_MAGIC_NUMBER)
    {
        err = FS_ERR_CORRUPT;
    }

    if(err == FS_ERR_OK
       && mFirmwarePackageMetadata.global.latest.firmwareNumber > FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        err = FS_ERR_CORRUPT;
    }

    if(err == FS_ERR_OK)
    {
        err = loadFirmwaresInformation();
    }

    if(err != FS_ERR_OK)
    {
        unmount();
    }

    return err;
}

int PackageFileSystem::unmount()
{
    memset(&mFirmwarePackageMetadata, 0, sizeof(mFirmwarePackageMetadata));
    return FS_ERR_OK;
}

int PackageFileSystem::remove(const std::string_view path)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::remove_recursive(const std::string_view path)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::rename(std::string_view oldpath, std::string_view newpath)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::stat(const std::string& path, fscommon::FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

eFirmwareFileIndex PackageFileSystem::getFirmwareIndex(std::string_view path) const
{
    ASSERT(memcmp(path.data(), "0", strlen("0")));
    ASSERT(memcmp(path.data(), "0", strlen("0")));

    /* Valid string index is from 1 to N. Integer index is string index - 1*/
    eFirmwareFileIndex index = atoi(path.data());
    if(index == 0 || index > mFirmwarePackageMetadata.global.latest.firmwareNumber)
    {
        index = FIRMWARE_MAX_AND_INVALID_INDEX;
    }
    else
    {
        --index;
    }

    return index;
}

bool PackageFileSystem::isMounted() const
{
    return (mFirmwarePackageMetadata.global.common.magicNumber == HEADER_MAGIC_NUMBER);
}


int PackageFileSystem::fileOpen(fscommon::FileHandle& file, std::string_view path, int flags)
{
    int err = FS_ERR_INVAL;
    AutoLock autoLock(mMutex);

    if(mFirmwarePackageMetadata.global.common.size == 0)
    {
        return FS_ERR_CORRUPT;
    }

    eFirmwareFileIndex handle = getFirmwareIndex(path);
    if(handle != FIRMWARE_MAX_AND_INVALID_INDEX && !mHandles[handle].used)
    {
        file = handle;

        memset(&mHandles[file], 0, sizeof(mHandles[file]));
        mHandles[file].used = true;
        mHandles[file].info = &(mFirmwarePackageMetadata.firmwares[file]);

        err = FS_ERR_OK;
    }

    return err;
}

int PackageFileSystem::fileClose(fscommon::FileHandle& file)
{
    AutoLock autoLock(mMutex);
    if(file < 0 || file >= FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        return FS_ERR_BADF;
    }

    memset(&mHandles[file], 0, sizeof(mHandles[file]));
    mHandles[file].used = false;

    return FS_ERR_OK;
}

int PackageFileSystem::fileSync(fscommon::FileHandle& file)
{
    return FS_ERR_NOIMPL;
}

bool PackageFileSystem::tryFileSeek(const fscommon::FileHandle& file, std::size_t off) const
{
    bool ret = true;

    ASSERT(mHandles[file].info != nullptr);
    if(off > mHandles[file].info->firmware_size)
    {
        ret = false;
    }

    return ret;
}


ssize_t PackageFileSystem::fileRead(fscommon::FileHandle& file, void* buffer, std::size_t size)
{
    if(file < 0 || file >= FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        return FS_ERR_BADF;
    }
    AutoLock autoLock(mMutex);


    ssize_t ret = FS_ERR_INVAL;

    if(!tryFileSeek(file, mHandles[file].offset + size))
    {
        size = mHandles[file].info->firmware_size - mHandles[file].offset;
    }

    const ssize_t offset = mHandles[file].info->firmware_offset + mHandles[file].offset;
    ret = (mFirmwarePackageFile.seek(offset, fs::SeekFlag::ABSOLUTE_POS) == offset ? FS_ERR_OK
                                                                                   : FS_ERR_INVAL);

    if(ret == FS_ERR_OK)
    {
        ret = mFirmwarePackageFile.read(buffer, size);
    }

    if(ret > 0)
    {
        mHandles[file].offset += ret;
    }

    return ret;
}

ssize_t PackageFileSystem::fileWrite(fscommon::FileHandle& file,
                                     const void* buffer,
                                     std::size_t size)
{
    return FS_ERR_NOIMPL;
}

ssize_t PackageFileSystem::fileSeek(fscommon::FileHandle& file, ssize_t off, fscommon::SeekFlag flag)
{
    if(file < 0 || file >= FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        return FS_ERR_BADF;
    }

    AutoLock autoLock(mMutex);
    ssize_t ret = FS_ERR_INVAL;
    size_t offset = 0;


    switch(flag)
    {
    case fscommon::SeekFlag::ABSOLUTE_POS:
        offset = off;
        ret = FS_ERR_OK;
        break;
    case fscommon::SeekFlag::CURRENT_POS:
        offset = mHandles[file].offset + off;
        ret = FS_ERR_OK;
        break;
    case fscommon::SeekFlag::END:
        offset = mHandles[file].info->firmware_size + off;
        ret = FS_ERR_OK;
        break;
    default:
        ret = FS_ERR_INVAL;
        break;
    }

    if(ret == FS_ERR_OK && tryFileSeek(file, offset))
    {
        mHandles[file].offset = offset;
        ret = mHandles[file].offset;
    }

    return ret;
}

int PackageFileSystem::fileTruncate(fscommon::FileHandle& file, std::size_t size)
{
    return FS_ERR_NOIMPL;
}

ssize_t PackageFileSystem::fileTell(fscommon::FileHandle& file)
{
    if(file < 0 || file >= FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        return FS_ERR_BADF;
    }

    AutoLock autoLock(mMutex);

    return mHandles[file].offset;
}

int PackageFileSystem::fileRewind(fscommon::FileHandle& file)
{
    if(file < 0 || file >= FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        return FS_ERR_BADF;
    }

    AutoLock autoLock(mMutex);

    mHandles[file].offset = 0;

    return FS_ERR_OK;
}

ssize_t PackageFileSystem::fileSize(fscommon::FileHandle& file)
{
    if(file < 0 || file >= FIRMWARE_MAX_AND_INVALID_INDEX)
    {
        return FS_ERR_BADF;
    }

    return static_cast<ssize_t>(mHandles[file].info->firmware_size);
}

int PackageFileSystem::mkdir(const char* path)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::dirOpen(fscommon::DirHandle& dir, std::string_view path)
{
    dir = INVALID_DIR_HANDLE;
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::dirClose(fscommon::DirHandle& dir)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::dirRead(fscommon::DirHandle& dir, fscommon::FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::dirSeek(fscommon::DirHandle& dir, std::size_t off)
{
    return FS_ERR_NOIMPL;
}

ssize_t PackageFileSystem::dirTell(fscommon::DirHandle& dir)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::dirRewind(fscommon::DirHandle& dir)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::findFirstFile(fscommon::DirHandle& dir,
                                     const std::string& path,
                                     const std::string& pattern,
                                     fscommon::FSInfo& info)
{
    dir = INVALID_DIR_HANDLE;
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::findNextFile(fscommon::DirHandle& dir,
                                    const std::string& pattern,
                                    fscommon::FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::traverse(fscommon::TraverseCallback& callback)
{
    return FS_ERR_NOIMPL;
}

std::size_t PackageFileSystem::getBlockCount() const
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::bdProg(std::size_t block,
                              std::size_t off,
                              const void* buffer,
                              std::size_t size)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::bdErase(std::size_t block)
{
    return FS_ERR_NOIMPL;
}

int PackageFileSystem::bdSync()
{
    return FS_ERR_NOIMPL;
}

void PackageFileSystem::lock()
{
    mMutex.lock();
}

void PackageFileSystem::unlock()
{
    mMutex.unlock();
}
