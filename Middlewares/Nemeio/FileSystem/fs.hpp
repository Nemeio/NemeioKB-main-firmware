/*
 * fs.hpp
 *
 *  Created on: 21 janv. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FILESYSTEM_FS_HPP_
#define NEMEIO_FILESYSTEM_FS_HPP_

/* None of the file below must include fs.hpp to use fs namespace ! */
#include "LittleFileSystem.hpp"
#include "IFileSystem.hpp"
#include "RawFileSystem.hpp"
#include "FileSystemCommon.hpp"
#include "FSInfo.hpp"
#include "File.hpp"
#include "Dir.hpp"

namespace fs
{
using namespace littlefs;
using namespace rawfs;
using namespace fscommon;

inline constexpr size_t MAX_PATH_SIZE = 256;
} // namespace fs


#endif /* NEMEIO_FILESYSTEM_FS_HPP_ */
