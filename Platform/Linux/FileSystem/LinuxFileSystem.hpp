#pragma once

#include "fs.hpp"
#include "Pool.hpp"

class LinuxFileSystem : public fscommon::IFileSystem
{
public:
    LinuxFileSystem() = default;
    virtual ~LinuxFileSystem() = default;

    int init() override { return fs::FS_ERR_OK; }
    int deinit() override { return fs::FS_ERR_OK; }
    int format() override { return fs::FS_ERR_OK; }
    int mount() override { return fs::FS_ERR_OK; }
    int unmount() override { return fs::FS_ERR_OK; }
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
    int fileTruncate(fscommon::FileHandle& file, std::size_t size) override
    {
        return fs::FS_ERR_NOIMPL;
    }
    ssize_t fileTell(fscommon::FileHandle& file) override { return fs::FS_ERR_NOIMPL; }
    int fileRewind(fscommon::FileHandle& file) override { return fs::FS_ERR_NOIMPL; }
    ssize_t fileSize(fscommon::FileHandle& file) override;
    int mkdir(const char* path) override;
    int dirOpen(fscommon::DirHandle& dir, std::string_view path) override;
    int dirClose(fscommon::DirHandle& dir) override;
    int dirRead(fscommon::DirHandle& dir, fscommon::FSInfo& info) override;
    int dirSeek(fscommon::DirHandle& dir, std::size_t off) override { return fs::FS_ERR_NOIMPL; }
    ssize_t dirTell(fscommon::DirHandle& dir) override { return fs::FS_ERR_NOIMPL; }
    int dirRewind(fscommon::DirHandle& dir) override { return fs::FS_ERR_NOIMPL; }
    int findFirstFile(fscommon::DirHandle& dir,
                      const std::string& path,
                      const std::string& pattern,
                      fscommon::FSInfo& info) override
    {
        return fs::FS_ERR_NOIMPL;
    }
    int findNextFile(fscommon::DirHandle& dir,
                     const std::string& pattern,
                     fscommon::FSInfo& info) override
    {
        return fs::FS_ERR_NOIMPL;
    }
    int traverse(fscommon::TraverseCallback& callback) override { return fs::FS_ERR_NOIMPL; }
    std::size_t getBlockCount() const override { return fs::FS_ERR_NOIMPL; }
    int bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size) override
    {
        return fs::FS_ERR_NOIMPL;
    }
    int bdProg(std::size_t block, std::size_t off, const void* buffer, std::size_t size) override
    {
        return fs::FS_ERR_NOIMPL;
    }
    int bdErase(std::size_t block) override { return fs::FS_ERR_NOIMPL; }
    int bdSync() override { return fs::FS_ERR_NOIMPL; }
    void lock() override {}
    void unlock() override {}
};
