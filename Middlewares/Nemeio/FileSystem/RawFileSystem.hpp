/*
 * RawFileSystem.hpp
 *
 *  Created on: 21 janv. 2021
 *      Author: thomas
 */
#ifndef FILESYSTEM_RAWFILESYSTEM_HPP_
#define FILESYSTEM_RAWFILESYSTEM_HPP_

#include <IBlockDevice.hpp>
#include <cstdint>
#include "WriteBuffer.hpp"
#include "IFileSystem.hpp"
#include "FileSystemCommon.hpp"
#include "FSInfo.hpp"
#include "RecursiveMutex.hpp"
#include "RawFileSystemHandle.hpp"
#include "IFileSystemCoordinator.hpp"
#include "Pool.hpp"
#include "Module.hpp"

namespace rawfs
{
inline constexpr char RAWFS_ROOT_PATH[] = "root";

class RawFileSystem : public fscommon::IFileSystem,
                      public Module<Identification::ModuleId::FILESYSTEM>
{
public:
    explicit RawFileSystem(IBlockDevice& blockDevice);
    virtual ~RawFileSystem() = default;

    int init() override;
    int deinit() override;
    int format() override;
    int mount() override;
    int unmount() override;
    int remove(std::string_view path) override;
    int remove_recursive(std::string_view path) override;
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
    static constexpr uint8_t DEFAULT_FLASH_VAL = 0xFF;

    /* Information about flash (should be set at runtime but we try to reduce the use of malloc) */
    static constexpr uint16_t FLASH_READ_SIZE = 256;
    static constexpr uint16_t FLASH_PROGRAM_SIZE = 256;

    Pool<RawFileSystemHandle<FLASH_READ_SIZE>, 2> mHandlesPool;

    uint8_t mReadBuffer[FLASH_READ_SIZE];

    IBlockDevice& mBlockDevice;
    RecursiveMutex mMutex;

    std::size_t getEraseBlockSize(void) const;
    std::size_t getProgramBlockSize(void) const;
    std::size_t getReadBlockSize(void) const;

    void resetBuffer(RawFileSystemHandle<FLASH_READ_SIZE>* handle);
    int flushBuffer(RawFileSystemHandle<FLASH_READ_SIZE>* handle);

    int newHandleIndex();
    void deleteHandle(RawFileSystemHandle<FLASH_READ_SIZE>* handle);
    RawFileSystemHandle<FLASH_READ_SIZE>* getFileHandleFromIndex(fscommon::FileHandle handle);
};
} // namespace rawfs

#endif /* FILESYSTEM_RAWFILESYSTEM_HPP_ */
