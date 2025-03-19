/*
 * FileSystemCommon.hpp
 *
 *  Created on: Nov 24, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_FILESYSTEMCOMMON_HPP_
#define NEMEIO_FILESYSTEM_FILESYSTEMCOMMON_HPP_

#include <cstdint>
#include <functional>

namespace fscommon
{
using TraverseCallback = std::function<int(std::size_t)>;

/* Error Code must be negative ! */
enum FSErrCode
{
    FS_ERR_IO = -127,       // Error during device operation
    FS_ERR_CORRUPT = -126,  // Corrupted
    FS_ERR_NOENT = -125,    // No directory entry
    FS_ERR_EXIST = -124,    // Entry already exists
    FS_ERR_NOTDIR = -123,   // Entry is not a dir
    FS_ERR_ISDIR = -122,    // Entry is a dir
    FS_ERR_NOTEMPTY = -121, // Dir is not empty
    FS_ERR_BADF = -120,     // Bad file number
    FS_ERR_INVAL = -119,    // Invalid parameter
    FS_ERR_NOSPC = -118,    // No space left on device
    FS_ERR_NOMEM = -117,    // No more memory available
    FS_ERR_NOIMPL = -116,   // Not implemented
    FS_ERR_SECURITY = -115, // Security error
    FS_ERR_OK = 0,          // No error
};

enum class FileType
{
    REG,
    DIR,
    SUPERBLOCK,
    UNDEFINED,
};

enum OpenFlag
{
    RDONLY = 1 << 0, // Open a file as read only
    WRONLY = 1 << 1, // Open a file as write only
    RDWR = 1 << 2,   // Open a file as read and write
    CREAT = 1 << 3,  // Create a file if it does not exist
    EXCL = 1 << 4,   // Fail if a file already exists
    TRUNC = 1 << 5,  // Truncate the existing file to zero size
    APPEND = 1 << 6, // Move to end of file on every write
};

enum class SeekFlag
{
    ABSOLUTE_POS, // Absolute position
    CURRENT_POS,  // Relative to current file position
    END,          // Relative to EOF
};

} // namespace fscommon

#endif /* NEMEIO_FILESYSTEM_FILESYSTEMCOMMON_HPP_ */
