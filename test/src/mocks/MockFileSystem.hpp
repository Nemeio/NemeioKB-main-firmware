#ifndef MOCKS_MOCKFILESYSTEM_HPP_
#define MOCKS_MOCKFILESYSTEM_HPP_

#include "gmock/gmock.h"
#include "fs.hpp"

#if defined(_MSC_VER)
typedef int ssize_t;
#endif

class MockFileSystem : public fs::IFileSystem
{
public:
    MOCK_METHOD(int, init, (), (override));
    MOCK_METHOD(int, deinit, (), (override));
    MOCK_METHOD(int, format, (), (override));
    MOCK_METHOD(int, mount, (), (override));
    MOCK_METHOD(int, unmount, (), (override));
    MOCK_METHOD(int, remove, (const std::string_view path), (override));
    MOCK_METHOD(int, remove_recursive, (const std::string_view path), (override));
    MOCK_METHOD(int, rename, (std::string_view oldpath, std::string_view newpath), (override));
    MOCK_METHOD(int, stat, (const std::string& path, fs::FSInfo& info), (override));
    MOCK_METHOD(int,
                fileOpen,
                (fs::FileHandle & file, std::string_view path, int flags),
                (override));
    MOCK_METHOD(int, fileClose, (fs::FileHandle & file), (override));
    MOCK_METHOD(int, fileSync, (fs::FileHandle & file), (override));
    MOCK_METHOD(ssize_t,
                fileRead,
                (fs::FileHandle & file, void* buffer, std::size_t size),
                (override));
    MOCK_METHOD(ssize_t,
                fileWrite,
                (fs::FileHandle & file, const void* buffer, std::size_t size),
                (override));
    MOCK_METHOD(ssize_t,
                fileSeek,
                (fs::FileHandle & file, ssize_t off, fs::SeekFlag flag),
                (override));
    MOCK_METHOD(int, fileTruncate, (fs::FileHandle & file, std::size_t size), (override));
    MOCK_METHOD(ssize_t, fileTell, (fs::FileHandle & file), (override));
    MOCK_METHOD(int, fileRewind, (fs::FileHandle & file), (override));
    MOCK_METHOD(ssize_t, fileSize, (fs::FileHandle & file), (override));
    MOCK_METHOD(int, mkdir, (const char* path), (override));
    MOCK_METHOD(int, dirOpen, (fs::DirHandle & dir, std::string_view path), (override));
    MOCK_METHOD(int, dirClose, (fs::DirHandle & dir), (override));
    MOCK_METHOD(int, dirRead, (fs::DirHandle & dir, fs::FSInfo& info), (override));
    MOCK_METHOD(int, dirSeek, (fs::DirHandle & dir, std::size_t off), (override));
    MOCK_METHOD(ssize_t, dirTell, (fs::DirHandle & dir), (override));
    MOCK_METHOD(int, dirRewind, (fs::DirHandle & dir), (override));
    MOCK_METHOD(int,
                findFirstFile,
                (fs::DirHandle & dir,
                 const std::string& path,
                 const std::string& pattern,
                 fs::FSInfo& info),
                (override));
    MOCK_METHOD(int,
                findNextFile,
                (fs::DirHandle & dir, const std::string& pattern, fs::FSInfo& info),
                (override));
    MOCK_METHOD(int, traverse, (fs::TraverseCallback & callback), (override));
    MOCK_METHOD(std::size_t, getBlockCount, (), (const, override));
    MOCK_METHOD(int,
                bdRead,
                (std::size_t block, std::size_t off, void* buffer, std::size_t size),
                (override));
    MOCK_METHOD(int,
                bdProg,
                (std::size_t block, std::size_t off, const void* buffer, std::size_t size),
                (override));
    MOCK_METHOD(int, bdErase, (std::size_t block), (override));
    MOCK_METHOD(int, bdSync, (), (override));
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
};

#endif /* MOCKS_MOCKFILESYSTEM_HPP_ */