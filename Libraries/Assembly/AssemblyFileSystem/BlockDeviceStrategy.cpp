/*
 * BlockDeviceStrategy.cpp
 *
 *  Created on: 9 déc. 2021
 *      Author: thomas
 */

#include <IBlockDeviceStrategy.hpp>

BlockDeviceStrategy::BlockDeviceStrategy(IBlockDevice& blockDevice)
    : mBlockDevice(blockDevice)
{
}

void BlockDeviceStrategy::setBlockDevice(IBlockDevice& blockDevice)
{
    mBlockDevice = blockDevice;
}

int BlockDeviceStrategy::init()
{
    return mBlockDevice.get().init();
}

int BlockDeviceStrategy::deinit()
{
    return mBlockDevice.get().deinit();
}

int BlockDeviceStrategy::sync()
{
    return mBlockDevice.get().sync();
}

int BlockDeviceStrategy::read(void* buffer, bd_addr_t addr, bd_size_t size)
{
    return mBlockDevice.get().read(buffer, addr, size);
}

int BlockDeviceStrategy::program(const void* buffer, bd_addr_t addr, bd_size_t size)
{
    return mBlockDevice.get().program(buffer, addr, size);
}

int BlockDeviceStrategy::erase(bd_addr_t addr, bd_size_t size)
{
    return mBlockDevice.get().erase(addr, size);
}

int BlockDeviceStrategy::trim(bd_addr_t addr, bd_size_t size)
{
    return mBlockDevice.get().trim(addr, size);
}

bd_size_t BlockDeviceStrategy::get_read_size() const
{
    return mBlockDevice.get().get_read_size();
}

bd_size_t BlockDeviceStrategy::get_program_size() const
{
    return mBlockDevice.get().get_program_size();
}

bd_size_t BlockDeviceStrategy::get_erase_size() const
{
    return mBlockDevice.get().get_erase_size();
}

bd_size_t BlockDeviceStrategy::get_erase_size(bd_addr_t addr) const
{
    return mBlockDevice.get().get_erase_size(addr);
}

int BlockDeviceStrategy::get_erase_value() const
{
    return mBlockDevice.get().get_erase_value();
}

bd_size_t BlockDeviceStrategy::size() const
{
    return mBlockDevice.get().size();
}

void BlockDeviceStrategy::lock()
{
    return mBlockDevice.get().lock();
}

void BlockDeviceStrategy::unlock()
{
    return mBlockDevice.get().unlock();
}
