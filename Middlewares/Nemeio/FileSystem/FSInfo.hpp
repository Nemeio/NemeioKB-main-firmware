/*
 * FSInfo.hpp
 *
 *  Created on: Nov 20, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FILESYSTEM_FSINFO_HPP_
#define NEMEIO_FILESYSTEM_FSINFO_HPP_

#include <cstddef>
#include <cstring>
#include <array>
#include "utils.hpp"
#include "FileSystemCommon.hpp"

namespace fscommon
{
struct FSInfo
{
    FSInfo() { std::fill(nameBuffer.begin(), nameBuffer.end(), '\0'); }

    FSInfo(FileType _type, std::size_t _size, const std::string& _name)
        : type(_type)
        , size(_size)
    {
        ASSERT(_name.length() < nameBuffer.size());
        std::copy(_name.begin(), _name.end(), nameBuffer.data());
        nameBuffer[_name.length()] = '\0';
    }

    FSInfo(const FSInfo&) = default;
    FSInfo(FSInfo&&) = default;
    FSInfo& operator=(const FSInfo&) = default;

    virtual ~FSInfo() = default;

    static constexpr std::size_t FILENAME_MAX_SIZE = 256;

    char* getName() { return nameBuffer.data(); }

    FileType type = FileType::REG;
    std::size_t size = 0;
    std::array<char, FILENAME_MAX_SIZE> nameBuffer;
};
} // namespace fscommon

#endif /* NEMEIO_FILESYSTEM_FSINFO_HPP_ */
