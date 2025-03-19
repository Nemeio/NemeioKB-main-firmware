/*
 * LittleFileSystem.cpp
 *
 *  Created on: Nov 19, 2020
 *      Author: eskoric
 */

#include "LittleFileSystem.hpp"
#include "AutoLock.h"
#include "IFileSystemCoordinator.hpp"

#include "FatFS_utils.hpp"
#include <memory>
#include <cstring>
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "lfs"

using namespace littlefs;
using namespace fscommon;

int LittleFileSystem::bd_read_callback(
    const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size)
{
    auto pLFS = reinterpret_cast<LittleFileSystem*>(c->context);
    ASSERT(pLFS != nullptr);

    return pLFS->bdRead(block, off, buffer, size) == BD_ERROR_OK ? 0 : -1;
}

int LittleFileSystem::bd_prog_callback(const struct lfs_config* c,
                                       lfs_block_t block,
                                       lfs_off_t off,
                                       const void* buffer,
                                       lfs_size_t size)
{
    auto pLFS = reinterpret_cast<LittleFileSystem*>(c->context);
    ASSERT(pLFS != nullptr);

    return pLFS->bdProg(block, off, buffer, size) == BD_ERROR_OK ? 0 : -1;
}

int LittleFileSystem::bd_erase_callback(const struct lfs_config* c, lfs_block_t block)
{
    auto pLFS = reinterpret_cast<LittleFileSystem*>(c->context);
    ASSERT(pLFS != nullptr);

    return pLFS->bdErase(block) == BD_ERROR_OK ? 0 : -1;
}

void LittleFileSystem::toFSInfo(const lfs_info& infoLFS, FSInfo& info) const
{
    ASSERT(info.nameBuffer.size() >= LFS_NAME_MAX + 1);

    info = FSInfo(toFSFileType(infoLFS.type), infoLFS.size, infoLFS.name);
}

std::size_t LittleFileSystem::getBlockCount() const
{
    return mLFSConfig.block_count;
}

int LittleFileSystem::bd_sync_callback(const struct lfs_config* c)
{
    auto pLFS = reinterpret_cast<LittleFileSystem*>(c->context);
    ASSERT(pLFS != nullptr);

    return pLFS->bdSync();
}

int LittleFileSystem::bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size)
{
    return mBlockDevice.read(buffer, (bd_addr_t) block * mLFSConfig.block_size + off, size);
}

int LittleFileSystem::bdProg(std::size_t block,
                             std::size_t off,
                             const void* buffer,
                             std::size_t size)
{
    mFileSystemCoordinator.notifyWrite();
    int err = mBlockDevice.program(buffer, (bd_addr_t) block * mLFSConfig.block_size + off, size);

    return err;
}

int LittleFileSystem::bdErase(std::size_t block)
{
    int ret = 0;

    auto readBuff = std::make_unique<uint8_t[]>(mLFSConfig.block_size);

    bdRead(block, 0, readBuff.get(), mLFSConfig.block_size);

    uint32_t i = 0;
    bool bErased = true;

    while(i < mLFSConfig.block_size && bErased)
    {
        if(readBuff[i] != 0xFF)
        {
            bErased = false;
            break;
        }
        ++i;
    }

    if(!bErased)
    {
        ret = mBlockDevice.erase((bd_addr_t) block * mLFSConfig.block_size, mLFSConfig.block_size);
    }

    return ret;
}

int LittleFileSystem::bdSync()
{
    return mBlockDevice.sync();
}

LittleFileSystem::LittleFileSystem(IFileSystemCoordinator& fileSystemCoordinator,
                                   IBlockDevice& blockDevice)
    : mFileSystemCoordinator(fileSystemCoordinator)
    , mBlockDevice(blockDevice)
{
    memset(&mLFS, 0, sizeof(mLFS));
    memset(&mLFSConfig, 0, sizeof(mLFSConfig));
}

int LittleFileSystem::format()
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);

    int err = toFSErrCode(lfs_format(&mLFS, &mLFSConfig));
    if(err != FS_ERR_OK)
    {
        LOG_ERR(MODULE,
                "Format error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                      static_cast<uint16_t>(-err))
                                << " on " << etl::hex << etl::showbase << this);
    }
    return err;
}

int LittleFileSystem::mount()
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);

    int err = toFSErrCode(lfs_mount(&mLFS, &mLFSConfig));
    if(err == FS_ERR_OK)
    {
        mIsMounted = true;
    }
    else
    {
        LOG_ERR(MODULE,
                "Mount error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                     static_cast<uint16_t>(-err))
                               << " on " << etl::hex << etl::showbase << this);
    }

    return err;
}

int LittleFileSystem::unmount()
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int err = FS_ERR_OK;

    if(mIsMounted)
    { /* Avoid multiple free if already unmounted */
        err = toFSErrCode(lfs_unmount(&mLFS));
    }

    return err;
}

int LittleFileSystem::remove(const std::string_view path)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);

    return toFSErrCode(lfs_remove(&mLFS, path.data()));
}

int LittleFileSystem::remove_recursive(const std::string_view path)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);

    return remove_recursive_n(path, MAX_RECURSION_LEVELS);
}

int LittleFileSystem::remove_recursive_n(const std::string_view path, uint8_t recursionLevels)
{
    if(recursionLevels == 0)
    {
        return FS_ERR_NOMEM;
    }
    --recursionLevels;

    if(int err = lfs_remove(&mLFS, path.data()); err != LFS_ERR_NOTEMPTY)
    {
        return toFSErrCode(err);
    }

    lfs_dir_t dir;
    if(int err = lfs_dir_open(&mLFS, &dir, path.data()); err != LFS_ERR_OK)
    {
        return toFSErrCode(err);
    }

    int err;
    bool remainingEntries;
    do
    {
        lfs_info info;
        err = lfs_dir_read(&mLFS, &dir, &info);
        remainingEntries = err > 0;
        if(remainingEntries)
        {
            std::string_view entryName(info.name);

            if(info.type != LFS_TYPE_DIR || ("." != entryName && ".." != entryName))
            {
                embedded_ostream subPath;
                subPath << path.data() << '/' << entryName.data();
                err = remove_recursive_n(subPath.str().data(), recursionLevels);
            }
        }
    } while(err >= 0 && remainingEntries);

    lfs_dir_close(&mLFS, &dir);

    if(err == LFS_ERR_OK)
    {
        err = lfs_remove(&mLFS, path.data());
    }

    return err;
}

int LittleFileSystem::rename(std::string_view oldpath, std::string_view newpath)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);

    return toFSErrCode(lfs_rename(&mLFS, oldpath.data(), newpath.data()));
}

int LittleFileSystem::stat(const std::string& path, FSInfo& info)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    lfs_info infoLFS;

    int errLFS = lfs_stat(&mLFS, path.c_str(), &infoLFS);

    toFSInfo(infoLFS, info);

    return toFSErrCode(errLFS);
}

int LittleFileSystem::fileOpen(fscommon::FileHandle& file, std::string_view path, int flags)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_NOMEM;
    file = newFileHandleIndex();

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        memset(handle, 0, sizeof(*handle));
        error = toFSErrCode(lfs_file_open(&mLFS, handle, path.data(), toLFSOpenFlag(flags)));
    }

    if(error != FS_ERR_OK)
    {
        deleteFileHandle(getFileHandleFromIndex(file));
        file = INVALID_FILE_HANDLE;
    }

    return error;
}

int LittleFileSystem::fileClose(fscommon::FileHandle& file)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;
    lfs_file_t* const handle = getFileHandleFromIndex(file);

    if(handle != nullptr)
    {
        error = toFSErrCode(lfs_file_close(&mLFS, handle));
    }

    deleteFileHandle(handle);
    file = INVALID_FILE_HANDLE;

    return error;
}

int LittleFileSystem::fileSync(fscommon::FileHandle& file)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_sync(&mLFS, handle));
    }

    return error;
}

ssize_t LittleFileSystem::fileRead(fscommon::FileHandle& file, void* buffer, std::size_t size)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    volatile ssize_t error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_read(&mLFS, handle, buffer, size));
    }

    if(error < FS_ERR_OK)
    {
        LOG_ERR(MODULE,
                "File read error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(-error))
                    << " on " << etl::hex << etl::showbase << this);
    }

    return error;
}

ssize_t LittleFileSystem::fileWrite(fscommon::FileHandle& file, const void* buffer, std::size_t size)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    ssize_t error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_write(&mLFS, handle, buffer, size));
    }

    if(error < FS_ERR_OK)
    {
        LOG_ERR(MODULE,
                "File write error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(-error))
                    << " on " << etl::hex << etl::showbase << this);
    }

    return error;
}

ssize_t LittleFileSystem::fileSeek(fscommon::FileHandle& file, ssize_t off, SeekFlag flag)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    ssize_t error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_seek(&mLFS, handle, off, toLFSSeekFlag(flag)));
    }

    return error;
}

int LittleFileSystem::fileTruncate(fscommon::FileHandle& file, std::size_t size)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_truncate(&mLFS, handle, size));
    }

    return error;
}

ssize_t LittleFileSystem::fileTell(fscommon::FileHandle& file)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    ssize_t error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_tell(&mLFS, handle));
    }

    return error;
}

int LittleFileSystem::fileRewind(fscommon::FileHandle& file)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_rewind(&mLFS, handle));
    }

    return error;
}

ssize_t LittleFileSystem::fileSize(fscommon::FileHandle& file)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_file_t* const handle = getFileHandleFromIndex(file); handle != nullptr)
    {
        error = toFSErrCode(lfs_file_size(&mLFS, handle));
    }

    return error;
}

int LittleFileSystem::mkdir(const char* path)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);

    return toFSErrCode(lfs_mkdir(&mLFS, path));
}

int LittleFileSystem::dirOpen(fscommon::DirHandle& dir, std::string_view path)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_NOMEM;
    dir = newFileHandleIndex();

    if(lfs_dir_t* const handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        error = toFSErrCode(lfs_dir_open(&mLFS, handle, path.data()));
    }

    return error;
}

int LittleFileSystem::dirClose(fscommon::DirHandle& dir)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;
    lfs_dir_t* const handle = getDirHandleFromIndex(dir);

    if(handle != nullptr)
    {
        error = toFSErrCode(lfs_dir_close(&mLFS, handle));
    }

    deleteDirHandle(handle);
    dir = INVALID_DIR_HANDLE;

    return error;
}

int LittleFileSystem::dirRead(fscommon::DirHandle& dir, FSInfo& info)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    lfs_info infoLFS;
    int error = FS_ERR_BADF;

    if(lfs_dir_t* const handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        error = toFSErrCode(lfs_dir_read(&mLFS, handle, &infoLFS));
        toFSInfo(infoLFS, info);
    }

    return error;
}

int LittleFileSystem::dirSeek(fscommon::DirHandle& dir, std::size_t off)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_dir_t* const handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        error = toFSErrCode(lfs_dir_seek(&mLFS, handle, off));
    }

    return error;
}

ssize_t LittleFileSystem::dirTell(fscommon::DirHandle& dir)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_dir_t* const handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        error = toFSErrCode(lfs_dir_tell(&mLFS, handle));
    }

    return error;
}

int LittleFileSystem::dirRewind(fscommon::DirHandle& dir)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int error = FS_ERR_BADF;

    if(lfs_dir_t* const handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        error = toFSErrCode(lfs_dir_rewind(&mLFS, handle));
    }

    return error;
}

int LittleFileSystem::init()
{
    init_cfg();
    return FS_ERR_OK;
}

int LittleFileSystem::deinit()
{
    return FS_ERR_OK;
}

void LittleFileSystem::init_cfg()
{
    /* TODO Enable thread safe */

    mLFSConfig.context = this;
    mLFSConfig.read = bd_read_callback;
    mLFSConfig.prog = bd_prog_callback;
    mLFSConfig.erase = bd_erase_callback;
    mLFSConfig.sync = bd_sync_callback;
    mLFSConfig.read_size = mBlockDevice.get_read_size();
    mLFSConfig.prog_size = mBlockDevice.get_program_size();
    mLFSConfig.block_size = mBlockDevice.get_erase_size();
    mLFSConfig.block_count = mBlockDevice.size() / mLFSConfig.block_size;
    mLFSConfig.cache_size = mLFSConfig.block_size;
    mLFSConfig.lookahead_size = 128;
    mLFSConfig.read_buffer = nullptr;
    mLFSConfig.prog_buffer = nullptr;
    mLFSConfig.lookahead_buffer = nullptr;
    mLFSConfig.block_cycles = 100;

    /* Use default values */
    mLFSConfig.name_max = 0;
    mLFSConfig.file_max = 0;
    mLFSConfig.attr_max = 0;
    mLFSConfig.metadata_max = 0;
}

int LittleFileSystem::findFirstFile(fscommon::DirHandle& dir,
                                    const std::string& path,
                                    const std::string& pattern,
                                    FSInfo& info)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int err = dirOpen(dir, path);

    if(err < 0)
    {
        return err;
    }

    if(lfs_dir_t* handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        err = findNextFile(dir, pattern, info);
    }

    return err;
}

int LittleFileSystem::findNextFile(fscommon::DirHandle& dir,
                                   const std::string& pattern,
                                   FSInfo& info)
{
    mFileSystemCoordinator.notifyFSRequest();
    AutoLock autoLock(mMutex);
    int errLFS = LFS_ERR_BADF;
    bool bFound = false;
    lfs_info info_lfs;

    if(lfs_dir_t* const handle = getDirHandleFromIndex(dir); handle != nullptr)
    {
        while(!bFound)
        {
            errLFS = (lfs_dir_read(&mLFS, handle, &info_lfs) == 1) ? LFS_ERR_OK : -1;
            if(errLFS < 0)
            {
                break;
            }
            else if(info_lfs.name[0] == '\0')
            {
                errLFS = LFS_ERR_NOENT;
                break;
            }
            else if(info_lfs.type == LFS_TYPE_REG
                    && pattern_matching(pattern.c_str(), info_lfs.name, 0, 0))
            {
                bFound = true;
            }
        }

        toFSInfo(info_lfs, info);
    }

    return toFSErrCode(errLFS);
}

int LittleFileSystem::traverse(TraverseCallback& callback)
{
    AutoLock autoLock(mMutex);
    return toFSErrCode(lfs_fs_traverse(&mLFS, traverseCallback, &callback));
}

int LittleFileSystem::toLFSSeekFlag(SeekFlag flag) const
{
    int lfsFlag = LFS_SEEK_SET;

    switch(flag)
    {
    case SeekFlag::ABSOLUTE_POS:
        lfsFlag = LFS_SEEK_SET;
        break;
    case SeekFlag::CURRENT_POS:
        lfsFlag = LFS_SEEK_CUR;
        break;
    case SeekFlag::END:
        lfsFlag = LFS_SEEK_END;
        break;
    default:
        ASSERT(false);
        break;
    }

    return lfsFlag;
}

int LittleFileSystem::toLFSOpenFlag(int flags) const
{
    int lfsFlag = 0;

    if((flags & OpenFlag::RDONLY) != 0)
    {
        lfsFlag |= LFS_O_RDONLY;
    }
    if((flags & OpenFlag::WRONLY) != 0)
    {
        lfsFlag |= LFS_O_WRONLY;
    }
    if((flags & OpenFlag::RDWR) != 0)
    {
        lfsFlag |= LFS_O_RDWR;
    }
    if((flags & OpenFlag::CREAT) != 0)
    {
        lfsFlag |= LFS_O_CREAT;
    }
    if((flags & OpenFlag::EXCL) != 0)
    {
        lfsFlag |= LFS_O_EXCL;
    }
    if((flags & OpenFlag::TRUNC) != 0)
    {
        lfsFlag |= LFS_O_TRUNC;
    }
    if((flags & OpenFlag::APPEND) != 0)
    {
        lfsFlag |= LFS_O_APPEND;
    }

    return lfsFlag;
}

FileType LittleFileSystem::toFSFileType(int fileTypeLFS) const
{
    FileType fileType = FileType::REG;

    switch(fileTypeLFS)
    {
    case LFS_TYPE_REG:
        fileType = FileType::REG;
        break;
    case LFS_TYPE_DIR:
        fileType = FileType::DIR;
        break;
    case LFS_TYPE_SUPERBLOCK:
        fileType = FileType::SUPERBLOCK;
        break;
    default:
        fileType = FileType::UNDEFINED;
        break;
    }

    return fileType;
}

void LittleFileSystem::lock()
{
    mMutex.lock();
}

void LittleFileSystem::unlock()
{
    mMutex.unlock();
}

int LittleFileSystem::traverseCallback(void* pCtx, lfs_block_t blockNum)
{
    auto pCallback = reinterpret_cast<TraverseCallback*>(pCtx);
    ASSERT(pCallback != nullptr);

    return (*pCallback)(blockNum);
}

int LittleFileSystem::toFSErrCode(int lfsErrCode) const
{
    int error;
    switch(lfsErrCode)
    {
    case LFS_ERR_OK:
        error = FS_ERR_OK;
        break;
    case LFS_ERR_IO:
        error = FS_ERR_IO;
        break;
    case LFS_ERR_CORRUPT:
        error = FS_ERR_CORRUPT;
        break;
    case LFS_ERR_NOENT:
        error = FS_ERR_NOENT;
        break;
    case LFS_ERR_EXIST:
        error = FS_ERR_EXIST;
        break;
    case LFS_ERR_NOTDIR:
        error = FS_ERR_NOTDIR;
        break;
    case LFS_ERR_ISDIR:
        error = FS_ERR_ISDIR;
        break;
    case LFS_ERR_NOTEMPTY:
        error = FS_ERR_NOTEMPTY;
        break;
    case LFS_ERR_BADF:
        error = FS_ERR_BADF;
        break;
    case LFS_ERR_INVAL:
        error = FS_ERR_INVAL;
        break;
    case LFS_ERR_NOSPC:
        error = FS_ERR_NOSPC;
        break;
    case LFS_ERR_NOMEM:
        error = FS_ERR_NOMEM;
        break;
    default:
        error = lfsErrCode;
        break;
    }
    return error;
}

int LittleFileSystem::newFileHandleIndex()
{
    int handle = mHandlesPool.newObjectIndex();
    if(handle < 0)
    {
        handle = INVALID_FILE_HANDLE;
    }
    return handle;
}

void LittleFileSystem::deleteFileHandle(lfs_file_t* handle)
{
    if(handle != nullptr)
    {
        mHandlesPool.deleteObject(reinterpret_cast<u_handles*>(handle));
        handle = nullptr;
    }
}

int LittleFileSystem::newDirHandleIndex()
{
    int handle = mHandlesPool.newObjectIndex();
    if(handle < 0)
    {
        handle = INVALID_DIR_HANDLE;
    }
    return handle;
}

void LittleFileSystem::deleteDirHandle(lfs_dir_t* handle)
{
    mHandlesPool.deleteObject(reinterpret_cast<u_handles*>(handle));
}

lfs_file_t* LittleFileSystem::getFileHandleFromIndex(FileHandle handle)
{
    if(handle == INVALID_FILE_HANDLE)
    {
        return nullptr;
    }
    return reinterpret_cast<lfs_file_t*>(mHandlesPool.getObject(handle));
}

lfs_dir_t* LittleFileSystem::getDirHandleFromIndex(FileHandle handle)
{
    if(handle == INVALID_DIR_HANDLE)
    {
        return nullptr;
    }
    return reinterpret_cast<lfs_dir_t*>(mHandlesPool.getObject(handle));
}
