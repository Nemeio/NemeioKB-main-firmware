/*
 * LinuxBlockDevice.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: Labo
 */

#include "LinuxBlockDevice.hpp"
#include <cstring>
#include <cassert>

LinuxBlockDevice::LinuxBlockDevice(std::string_view file)
{
    mFile = fopen(file.data(), "rw");
    assert(mFile != nullptr);
    memset(mEraseBuffer, get_erase_value(), sizeof(mEraseBuffer));
}

int LinuxBlockDevice::init()
{
    return 0;
}

int LinuxBlockDevice::deinit()
{
    return 0;
}

int LinuxBlockDevice::read(void* buffer, bd_addr_t addr, bd_size_t size)
{
    assert(is_valid_read(addr, size));
    assert(mFile != nullptr);

    fseek(mFile, addr, SEEK_SET);
    memset(buffer, get_erase_value(), size);

    auto a = fread(buffer, 1, size, mFile);

    return a >= 0 ? 0 : -1;
}

int LinuxBlockDevice::program(const void* buffer, bd_addr_t addr, bd_size_t size)
{
    assert(is_valid_program(addr, size));
    assert(mFile != NULL);
    fseek(mFile, addr, SEEK_SET);
    return fwrite(buffer, 1, size, mFile) == size ? 0 : -1;
}

int LinuxBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    assert(is_valid_erase(addr, size));
    assert(mFile != nullptr);

    fseek(mFile, addr, SEEK_SET);
    return fwrite(mEraseBuffer, 1, size, mFile) == size ? 0 : -1;
}

bd_size_t LinuxBlockDevice::get_read_size() const
{
    return sReadSize;
}

bd_size_t LinuxBlockDevice::get_program_size() const
{
    return sProgSize;
}

bd_size_t LinuxBlockDevice::get_erase_size() const
{
    return sEraseSize;
}

bd_size_t LinuxBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return sEraseSize;
}

int LinuxBlockDevice::get_erase_value() const
{
    return 0xFF;
}

bd_size_t LinuxBlockDevice::size() const
{
    return sSize;
}

void LinuxBlockDevice::lock()
{
    mMutex.lock();
}

void LinuxBlockDevice::unlock()
{
    mMutex.unlock();
}
