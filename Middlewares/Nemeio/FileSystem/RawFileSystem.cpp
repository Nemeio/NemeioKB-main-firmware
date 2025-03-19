/*
 * RawFileSystem.cpp
 *
 *  Created on: 21 janv. 2021
 *      Author: thomas
 */

#include "RawFileSystem.hpp"
#include "AutoLock.h"
#include "assertTools.h"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "rawfs"

using namespace rawfs;
using namespace fscommon;

RawFileSystem::RawFileSystem(IBlockDevice& blockDevice)
    : mBlockDevice(blockDevice)
{
}

int RawFileSystem::init()
{
    ASSERT(getReadBlockSize() == sizeof(mReadBuffer));
    return FS_ERR_OK;
}

int RawFileSystem::deinit()
{
    return FS_ERR_OK;
}

int RawFileSystem::format()
{
    AutoLock autoLock(mMutex);
    int ret = 0;
    size_t nbBlocks = getBlockCount();

    for(uint32_t block = 0; block < nbBlocks; ++block)
    {
        ret = bdErase(block);
        if(ret != 0)
        {
            break;
        }
    }

    int err = (ret == 0) ? FS_ERR_OK : FS_ERR_IO;

    if(err != FS_ERR_OK)
    {
        LOG_ERR(MODULE,
                "Format error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                      static_cast<uint16_t>(-err))
                                << "on " << etl::hex << etl::showbase << this);
    };
    return err;
}

int RawFileSystem::mount()
{
    return FS_ERR_OK;
}

int RawFileSystem::unmount()
{
    return FS_ERR_OK;
}

int RawFileSystem::remove(std::string_view path)
{
    int ret = FS_ERR_INVAL;
    if(path == RAWFS_ROOT_PATH)
    {
        ret = format();
    }
    return ret;
}

int RawFileSystem::remove_recursive(const std::string_view path)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::rename(std::string_view oldpath, std::string_view newpath)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::stat(const std::string& path, FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::fileOpen(FileHandle& file, std::string_view path, int flags)
{
    int err = FS_ERR_INVAL;
    AutoLock autoLock(mMutex);

    if(path == RAWFS_ROOT_PATH)
    {
        file = newHandleIndex();
        if(auto const handle = getFileHandleFromIndex(file); handle != nullptr)
        {
            handle->setOffset(0);
            handle->reset();
            err = FS_ERR_OK;
        }
    }
    else
    {
        file = INVALID_FILE_HANDLE;
    }

    return err;
}

int RawFileSystem::fileClose(FileHandle& file)
{
    int err = FS_ERR_BADF;

    if(auto const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        flushBuffer(handle);
        deleteHandle(handle);
        err = FS_ERR_OK;
    }

    file = INVALID_FILE_HANDLE;

    return err;
}

int RawFileSystem::fileSync(FileHandle& file)
{
    return FS_ERR_NOIMPL;
}

ssize_t RawFileSystem::fileRead(FileHandle& file, void* buffer, std::size_t size)
{
    AutoLock autoLock(mMutex);

    auto pBuffer = reinterpret_cast<uint8_t*>(buffer);
    ssize_t nbBytesRead = 0;

    auto const handle = getFileHandleFromIndex(file);
    if(handle == nullptr)
    {
        return FS_ERR_BADF;
    }

    if(handle->getOffset() + size >= this->mBlockDevice.size())
    {
        return FS_ERR_INVAL;
    }

    /* We need to read data with an alignement given by "read block size" */

    /* Get the position of the current offset using block */
    size_t blockIndex = handle->getOffset() / getReadBlockSize();
    size_t blockOffset = handle->getOffset() % getReadBlockSize();

    size_t remainingSize = size;
    while(remainingSize > 0 && !bdRead(blockIndex, 0, mReadBuffer, sizeof(mReadBuffer)))
    {
        const size_t neededDataSize = std::min(remainingSize, sizeof(mReadBuffer) - blockOffset);
        memcpy(pBuffer, mReadBuffer + blockOffset, neededDataSize);
        blockOffset =
            0; /* Only the first iteration we need to have an offset, then we read entire blocks */
        blockIndex++;
        remainingSize -= neededDataSize;
        pBuffer += neededDataSize;
    }

    nbBytesRead = size - remainingSize;
    handle->addOffset(nbBytesRead);

    return nbBytesRead;
}

ssize_t RawFileSystem::fileWrite(FileHandle& file, const void* buffer, std::size_t size)
{
    AutoLock autoLock(mMutex);

    auto const handle = getFileHandleFromIndex(file);
    if(handle == nullptr)
    {
        return FS_ERR_BADF;
    }

    if(handle->getOffset() + size >= this->mBlockDevice.size())
    {
        return FS_ERR_INVAL;
    }

    /* NOT SUPPORTED: You can't use fileSeek when writing the file ! */
    auto pData = reinterpret_cast<const uint8_t*>(buffer);
    int ret = FS_ERR_OK;
    std::size_t remainingDataLen = size;

    /* We need to write in flash using a multiple of program size, we use a buffer to make sure of this */

    /* If we already have data buffered, we complete it (to have a multiple of program size) */
    if(!handle->isEmpty())
    {
        const std::size_t copyLen = std::min(size, handle->remainingSpace());
        const std::size_t bufferedLen = handle->pushBack(pData, pData + copyLen);
        ASSERT(bufferedLen == copyLen);

        pData += copyLen;
        remainingDataLen -= copyLen;

        handle->addOffset(copyLen);
    }

    /* If buffer is full, we write into flash */
    if(handle->isFull())
    {
        ret = flushBuffer(handle);
    }

    /* Write data into flash */
    if(FS_ERR_OK == ret && remainingDataLen)
    {
        const uint32_t blockIndex = handle->getOffset() / getProgramBlockSize();
        const uint32_t blockOffset = handle->getOffset() % getProgramBlockSize();
        const std::size_t writeLen = (remainingDataLen / getProgramBlockSize())
                                     * getProgramBlockSize();
        ret = bdProg(blockIndex, blockOffset, pData, writeLen);
        if(ret == 0)
        {
            remainingDataLen -= writeLen;
            pData += writeLen;
            handle->addOffset(writeLen);

            /* If we still have data (not multiple of the program size, we insert them in the buffer */
            if(remainingDataLen)
            {
                std::size_t bufferedLen = handle->pushBack(pData, pData + remainingDataLen);
                ASSERT(bufferedLen == remainingDataLen);
                handle->addOffset(remainingDataLen);
                remainingDataLen = 0;
            }
        }
    }

    return (size - remainingDataLen);
}

void RawFileSystem::resetBuffer(RawFileSystemHandle<FLASH_READ_SIZE>* handle)
{
    handle->reset(DEFAULT_FLASH_VAL);
}

int RawFileSystem::flushBuffer(RawFileSystemHandle<FLASH_READ_SIZE>* handle)
{
    if(handle == nullptr || handle->isEmpty())
    {
        return FS_ERR_OK;
    }

    ASSERT(handle->usedSpace() <= handle->getOffset());
    const uint32_t blockIndex = (handle->getOffset() - handle->usedSpace()) / getProgramBlockSize();
    const uint32_t blockOffset = (handle->getOffset() - handle->usedSpace())
                                 % getProgramBlockSize();

    int ret = bdProg(blockIndex, blockOffset, handle->data(), handle->size()) == 0 ? FS_ERR_OK
                                                                                   : FS_ERR_IO;

    resetBuffer(handle);


    return ret;
}

ssize_t RawFileSystem::fileSeek(FileHandle& file, ssize_t off, SeekFlag flag)
{
    AutoLock autoLock(mMutex);
    ssize_t ret = FS_ERR_INVAL;

    auto const handle = getFileHandleFromIndex(file);
    if(handle == nullptr)
    {
        return FS_ERR_BADF;
    }
    flushBuffer(handle);

    switch(flag)
    {
    case fscommon::SeekFlag::ABSOLUTE_POS:
        handle->setOffset(off);
        ret = handle->getOffset();
        break;
    case fscommon::SeekFlag::CURRENT_POS:
        ret = FS_ERR_NOIMPL;
        break;
    case fscommon::SeekFlag::END:
        ret = FS_ERR_NOIMPL;
        break;
    default:
        ret = FS_ERR_INVAL;
        break;
    }

    return ret;
}

int RawFileSystem::fileTruncate(FileHandle& file, std::size_t size)
{
    return FS_ERR_NOIMPL;
}

ssize_t RawFileSystem::fileTell(FileHandle& file)
{
    ssize_t ret = FS_ERR_BADF;

    if(auto const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        ret = handle->getOffset();
    }

    return ret;
}

int RawFileSystem::fileRewind(FileHandle& file)
{
    AutoLock autoLock(mMutex);
    int err = FS_ERR_BADF;

    if(auto const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        flushBuffer(handle);
        handle->setOffset(0);
        err = FS_ERR_OK;
    }
    return err;
}

ssize_t RawFileSystem::fileSize(FileHandle& file)
{
    return mBlockDevice.size();
}

int RawFileSystem::mkdir(const char* path)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::dirOpen(DirHandle& dir, std::string_view path)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::dirClose(DirHandle& dir)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::dirRead(DirHandle& dir, FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::dirSeek(DirHandle& dir, std::size_t off)
{
    return FS_ERR_NOIMPL;
}

ssize_t RawFileSystem::dirTell(DirHandle& dir)
{
    return -1;
}

int RawFileSystem::dirRewind(DirHandle& dir)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::findFirstFile(DirHandle& dir,
                                 const std::string& path,
                                 const std::string& pattern,
                                 FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::findNextFile(DirHandle& dir, const std::string& pattern, FSInfo& info)
{
    return FS_ERR_NOIMPL;
}

int RawFileSystem::traverse(TraverseCallback& callback)
{
    return FS_ERR_NOIMPL;
}

std::size_t RawFileSystem::getBlockCount() const
{
    return mBlockDevice.size() / getEraseBlockSize();
}

int RawFileSystem::bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size)
{
    ASSERT(off == 0);
    return mBlockDevice.read(buffer, static_cast<bd_addr_t>(block * getReadBlockSize() + off), size)
                   == BD_ERROR_OK
               ? 0
               : -1;
}

int RawFileSystem::bdProg(std::size_t block, std::size_t off, const void* buffer, std::size_t size)
{
    ASSERT(off == 0);
    return mBlockDevice.program(buffer,
                                static_cast<bd_addr_t>(block * getProgramBlockSize() + off),
                                size)
                   == BD_ERROR_OK
               ? 0
               : -1;
}

int RawFileSystem::bdErase(std::size_t block)
{
    return mBlockDevice.erase(block * getEraseBlockSize(), mBlockDevice.get_erase_size())
                   == BD_ERROR_OK
               ? 0
               : -1;
}

int RawFileSystem::bdSync()
{
    return mBlockDevice.sync();
}

void RawFileSystem::lock()
{
    mMutex.lock();
}

void RawFileSystem::unlock()
{
    mMutex.unlock();
}

std::size_t RawFileSystem::getEraseBlockSize(void) const
{
    return mBlockDevice.get_erase_size();
}

std::size_t RawFileSystem::getProgramBlockSize(void) const
{
    return mBlockDevice.get_program_size();
}

std::size_t RawFileSystem::getReadBlockSize(void) const
{
    return mBlockDevice.get_read_size();
}

int RawFileSystem::newHandleIndex()
{
    int handle = mHandlesPool.newObjectIndex();
    if(handle < 0)
    {
        handle = INVALID_FILE_HANDLE;
    }
    return handle;
}

void RawFileSystem::deleteHandle(RawFileSystemHandle<RawFileSystem::FLASH_READ_SIZE>* handle)
{
    mHandlesPool.deleteObject(handle);
}

RawFileSystemHandle<RawFileSystem::FLASH_READ_SIZE>* RawFileSystem::getFileHandleFromIndex(
    FileHandle handle)
{
    if(handle == INVALID_FILE_HANDLE)
    {
        return nullptr;
    }
    return mHandlesPool.getObject(handle);
}
