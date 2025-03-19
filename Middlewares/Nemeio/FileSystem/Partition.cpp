/*
 * Partition.cpp
 *
 *  Created on: 25 mars 2021
 *      Author: thomas
 */

#include <algorithm>
#include "Partition.hpp"
#include "assertTools.h"

Partition::Partition(IBlockDevice& blockDevice, const uint32_t startAddress, const bd_size_t size)
    : mBlockDevice(blockDevice)
    , mOffset(startAddress)
    , mSize(size)
{
}

int Partition::init()
{
    /* Check alignment */
    ASSERT(is_valid_erase(0, std::min(mSize, get_erase_size())));
    ASSERT(is_valid_read(0, std::min(mSize, get_read_size())));
    ASSERT(is_valid_program(0, std::min(mSize, get_program_size())));

    return 0;
}

int Partition::deinit()
{
    return 0;
}

int Partition::read(void* buffer, bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_read(addr, size));
    return mBlockDevice.read(buffer, mOffset + addr, size);
}

int Partition::program(const void* buffer, bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_program(addr, size));
    return mBlockDevice.program(buffer, mOffset + addr, size);
}

int Partition::erase(bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_erase(addr, size));
    return mBlockDevice.erase(mOffset + addr, size);
}

bd_size_t Partition::get_read_size() const
{
    return mBlockDevice.get_read_size();
}

bd_size_t Partition::get_program_size() const
{
    return mBlockDevice.get_program_size();
}

bd_size_t Partition::get_erase_size() const
{
    return mBlockDevice.get_erase_size();
}

bd_size_t Partition::get_erase_size(bd_addr_t addr) const
{
    return mBlockDevice.get_erase_size(addr);
}

int Partition::get_erase_value() const
{
    return mBlockDevice.get_erase_value();
}

bd_size_t Partition::size() const
{
    return mSize;
}
