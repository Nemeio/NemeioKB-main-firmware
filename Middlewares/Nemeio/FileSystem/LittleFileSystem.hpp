/*
 * FileSystem.hpp
 *
 *  Created on: Nov 19, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_LITTLEFILESYSTEM_HPP_
#define NEMEIO_FILESYSTEM_LITTLEFILESYSTEM_HPP_

#include <IBlockDevice.hpp>
#include <cstddef>
#include <functional>

#include "RecursiveMutex.hpp"
#include "lfs.h"
#include "FileSystemCommon.hpp"
#include "FSInfo.hpp"
#include "IFileSystem.hpp"
#include "IFileSystemCoordinator.hpp"
#include "../Tools/Pool.hpp"
#include "Module.hpp"

constexpr uint8_t LFS_MAX_HANDLES = 5;

namespace littlefs
{
class LittleFileSystem : public fscommon::IFileSystem,
                         public Module<Identification::ModuleId::FILESYSTEM>
{
public:
    LittleFileSystem(fscommon::IFileSystemCoordinator& fileSystemCoordinator,
                     IBlockDevice& blockDevice);
    virtual ~LittleFileSystem() = default;

    int init() override;
    int deinit() override;
    int format() override;
    int mount() override;
    int unmount() override;
    int remove(const std::string_view path) override;
    int remove_recursive(const std::string_view path) override;
    int rename(std::string_view oldpath, std::string_view newpath) override;
    int stat(const std::string& path, fscommon::FSInfo& info) override;
    int fileOpen(fscommon::FileHandle& file, std::string_view path, int flags) override;
    int fileClose(fscommon::FileHandle& file) override;
    int fileSync(fscommon::FileHandle& file) override;
    ssize_t fileRead(fscommon::FileHandle& file, void* buffer, std::size_t size) override;
    ssize_t fileWrite(fscommon::FileHandle& file, const void* buffer, std::size_t size) override;
    ssize_t fileSeek(fscommon::FileHandle& file, ssize_t off, fscommon::SeekFlag flag) override;
    int fileTruncate(fscommon::FileHandle& file, std::size_t size) override;
    ssize_t fileTell(fscommon::FileHandle& file) override;
    int fileRewind(fscommon::FileHandle& file) override;
    ssize_t fileSize(fscommon::FileHandle& file) override;
    int mkdir(const char* path) override;
    int dirOpen(fscommon::DirHandle& dir, std::string_view path) override;
    int dirClose(fscommon::DirHandle& dir) override;
    int dirRead(fscommon::DirHandle& dir, fscommon::FSInfo& info) override;
    int dirSeek(fscommon::DirHandle& dir, std::size_t off) override;
    ssize_t dirTell(fscommon::DirHandle& dir) override;
    int dirRewind(fscommon::DirHandle& dir) override;
    int findFirstFile(fscommon::DirHandle& dir,
                      const std::string& path,
                      const std::string& pattern,
                      fscommon::FSInfo& info) override;
    int findNextFile(fscommon::DirHandle& dir,
                     const std::string& pattern,
                     fscommon::FSInfo& info) override;
    int traverse(fscommon::TraverseCallback& callback) override;
    std::size_t getBlockCount() const override;
    int bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size) override;
    int bdProg(std::size_t block, std::size_t off, const void* buffer, std::size_t size) override;
    int bdErase(std::size_t block) override;
    int bdSync() override;
    void lock() override;
    void unlock() override;

private:
    static constexpr uint8_t MAX_RECURSION_LEVELS = 3;
    fscommon::IFileSystemCoordinator& mFileSystemCoordinator;
    IBlockDevice& mBlockDevice;
    RecursiveMutex mMutex;
    bool mIsMounted = false;
    lfs_t mLFS;
    lfs_config mLFSConfig;
    union u_handles
    {
        lfs_file_t file;
        lfs_dir_t dir;
    };
    Pool<u_handles, LFS_MAX_HANDLES> mHandlesPool;

    void init_cfg();

    void toFSInfo(const lfs_info& infoLFS, fscommon::FSInfo& info) const;
    [[nodiscard]] int toFSErrCode(int lfsErrCode) const;
    [[nodiscard]] fscommon::FileType toFSFileType(int fileTypeLFS) const;

    [[nodiscard]] int toLFSOpenFlag(int flags) const;
    [[nodiscard]] int toLFSSeekFlag(fscommon::SeekFlag flag) const;

    static int bd_read_callback(const struct lfs_config* c,
                                lfs_block_t block,
                                lfs_off_t off,
                                void* buffer,
                                lfs_size_t size);

    static int bd_prog_callback(const struct lfs_config* c,
                                lfs_block_t block,
                                lfs_off_t off,
                                const void* buffer,
                                lfs_size_t size);

    static int bd_erase_callback(const struct lfs_config* c, lfs_block_t block);

    static int bd_sync_callback(const struct lfs_config* c);

    static int traverseCallback(void* pCtx, lfs_block_t blockNum);

    int newFileHandleIndex();
    void deleteFileHandle(lfs_file_t* handle);
    int newDirHandleIndex();
    void deleteDirHandle(lfs_dir_t* handle);
    lfs_file_t* getFileHandleFromIndex(fscommon::FileHandle handle);
    lfs_dir_t* getDirHandleFromIndex(fscommon::FileHandle handle);
    int remove_recursive_n(std::string_view path, uint8_t recursionLevels);
};
} // namespace littlefs

#endif /* NEMEIO_FILESYSTEM_LITTLEFILESYSTEM_HPP_ */
