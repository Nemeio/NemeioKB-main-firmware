/*
 * IFileSystem.hpp
 *
 *  Created on: Nov 18, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_IFILESYSTEM_HPP_
#define NEMEIO_FILESYSTEM_IFILESYSTEM_HPP_

#include <string>
#include <cstddef>
#include <cstdint>
#include <functional>
#include "FileSystemCommon.hpp"
#include "FSInfo.hpp"

namespace fscommon
{
using FileHandle = size_t;
using DirHandle = size_t;
inline constexpr FileHandle INVALID_FILE_HANDLE = -1;
inline constexpr DirHandle INVALID_DIR_HANDLE = -1;

class IFileSystem
{
public:
    virtual int init() = 0;
    virtual int deinit() = 0;

    /// Filesystem functions ///

    // Format a block device with the littlefs
    //
    // Requires a littlefs object and config struct. This clobbers the littlefs
    // object, and does not leave the filesystem mounted. The config struct must
    // be zeroed for defaults and backwards compatibility.
    //<functional>
    // Returns a negative error code on failure.
    virtual int format() = 0;

    // Mounts a littlefs
    //
    // Requires a littlefs object and config struct. Multiple filesystems
    // may be mounted simultaneously with multiple littlefs objects. Both
    // lfs and config must be allocated while mounted. The config struct must
    // be zeroed for defaults and backwards compatibility.
    //
    // Returns a negative error code on failure.
    virtual int mount() = 0;

    // Unmounts a littlefs
    //
    // Does nothing besides releasing any allocated resources.
    // Returns a negative error code on failure.
    virtual int unmount() = 0;

    /// General operations ///

    // Removes a file or directory
    //
    // If removing a directory, the directory must be empty.
    // Returns a negative error code on failure.
    virtual int remove(const std::string_view path) = 0;

    // Removes a directory recursively
    //
    // Returns a negative error code on failure.
    virtual int remove_recursive(const std::string_view path) = 0;

    // Rename or move a file or directory
    //
    // If the destination exists, it must match the source in type.
    // If the destination is a directory, the directory must be empty.
    //
    // Returns a negative error code on failure.
    virtual int rename(std::string_view oldpath, std::string_view newpath) = 0;

    // Find info about a file or directory
    //
    // Fills out the info structure, based on the specified file or directory.
    // Returns a negative error code on failure.
    virtual int stat(const std::string& path, FSInfo& info) = 0;

    /// File operations ///

    // Open a file
    //
    // The mode that the file is opened in is determined by the flags, which
    // are values from the enum lfs_open_flags that are bitwise-ored together.
    //
    // Returns a negative error code on failure.
    virtual int fileOpen(FileHandle& file, std::string_view path, int flags) = 0;

    // Close a file
    //
    // Any pending writes are written out to s<cstddef>torage as though
    // sync had been called and releases any allocated resources.
    //
    // Returns a negative error code on failure.
    virtual int fileClose(FileHandle& file) = 0;

    // Synchronize a file on storage
    //
    // Any pending writes are written out to storage.
    // Returns a negative error code on failure.
    virtual int fileSync(FileHandle& file) = 0;

    // Read data from file
    //
    // Takes a buffer and size indicating where to store the read data.
    // Returns the number of bytes read, or a negative error code on failure.
    virtual ssize_t fileRead(FileHandle& file, void* buffer, std::size_t size) = 0;

    // Write data to file
    //
    // Takes a buffer and size indicating the data to write. The file will not
    // actually be updated on the storage until either sync or close is called.
    //
    // Returns the number of bytes written, or a negative error code on failure.
    virtual ssize_t fileWrite(FileHandle& file, const void* buffer, std::size_t size) = 0;

    // Change the position of the file
    //
    // The change in position is determined by the offset and whence flag.
    // Returns the old position of the file, or a negative error code on failure.
    virtual ssize_t fileSeek(FileHandle& file, ssize_t off, SeekFlag flag) = 0;

    // Truncates the size of the file to the specified size
    //
    // Returns a negative error code on failure.
    virtual int fileTruncate(FileHandle& file, std::size_t size) = 0;

    // Return the position of the file
    //
    // Equivalent to lfs_file_seek(lfs, file, <cstddef>0, LFS_SEEK_CUR)
    // Returns the position of the file, or a negative error code on failure.
    virtual ssize_t fileTell(FileHandle& file) = 0;

    // Change the position of the file to the beginning of the file
    //
    // Equivalent to lfs_file_seek(lfs, file, 0, LFS_SEEK_CUR)
    // Returns a negative error code on failure.
    virtual int fileRewind(FileHandle& file) = 0;

    // Return the size of the file
    //
    // Similar to lfs_file_seek(lfs, file, 0, LFS_SEEK_END)
    // Returns the size of the file, or a negative error code on failure.
    virtual ssize_t fileSize(FileHandle& file) = 0;

    /// Directory operations ///

    // Create a directory
    //
    // Returns a negative error code on failure.
    virtual int mkdir(const char* path) = 0;

    // Open a directory
    //
    // Once open a directory can be used with read to iterate over files.
    // Returns a negative error code on failure.
    virtual int dirOpen(DirHandle& dir, std::string_view path) = 0;

    // Close a directory
    //
    // Releases any allocated resources.
    // Returns a negative error code on failure.
    virtual int dirClose(DirHandle& dir) = 0;

    // Read an entry in the directory
    //
    // Fills out the info structure, based on the specified file or directory.
    // Returns a negative error code on failure.
    virtual int dirRead(DirHandle& dir, FSInfo& info) = 0;

    // Change the position of the directory
    //
    // The new off must be a value previous returned from tell and specifies
    // an absolute offset in the directobdProgry seek.
    //
    // Returns a negative error code on failure.
    virtual int dirSeek(DirHandle& dir, std::size_t off) = 0;

    // Return the position of the directory
    //
    // The returned offset is only meant to be consumed by seek and may not make
    // sense, but does indicate the current position in the directory iteration.
    //
    // Returns the position of the directory, or a negative error code on failure.
    virtual ssize_t dirTell(DirHandle& dir) = 0;

    // Change the position of the directory to the beginning of the directory
    //
    // Returns a negative error code on failure.
    virtual int dirRewind(DirHandle& dir) = 0;

    virtual int findFirstFile(DirHandle& dir,
                              const std::string& path,
                              const std::string& pattern,
                              FSInfo& info) = 0;

    virtual int findNextFile(DirHandle& dir, const std::string& pattern, FSInfo& info) = 0;

    virtual int traverse(TraverseCallback& callback) = 0;

    [[nodiscard]] virtual std::size_t getBlockCount() const = 0;

    ////// Block device operations //////
    virtual int bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size) = 0;

    virtual int bdProg(std::size_t block, std::size_t off, const void* buffer, std::size_t size) = 0;

    virtual int bdErase(std::size_t block) = 0;

    virtual int bdSync() = 0;

    virtual void lock() = 0;
    virtual void unlock() = 0;
};

};     // namespace fscommon
#endif /* NEMEIO_FILESYSTEM_IFILESYSTEM_HPP_ */
