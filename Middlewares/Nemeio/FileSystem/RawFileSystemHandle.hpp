/*
 * RawFileSystemHandle.hpp
 *
 *  Created on: 3 févr. 2021
 *      Author: thomas
 */

#ifndef FILESYSTEM_RAWFILESYSTEMHANDLE_HPP_
#define FILESYSTEM_RAWFILESYSTEMHANDLE_HPP_

#include <cstddef>
#include <cstdint>
#include "WriteBuffer.hpp"

namespace rawfs
{
template<std::size_t SIZE>
class RawFileSystemHandle : public WriteBuffer<uint8_t, SIZE>
{
public:
    RawFileSystemHandle() = default;
    virtual ~RawFileSystemHandle() = default;

    void setOffset(size_t offset) { mOffset = offset; };
    size_t getOffset() const { return mOffset; };
    void addOffset(size_t offset) { mOffset += offset; };

private:
    size_t mOffset = 0;
};

} /* namespace rawfs */

#endif /* FILESYSTEM_RAWFILESYSTEMHANDLE_HPP_ */
