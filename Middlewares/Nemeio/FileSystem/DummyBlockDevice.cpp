/*
 * DummyBlockDevice.cpp
 *
 *  Created on: 9 déc. 2021
 *      Author: thomas
 */

#include <DummyIBlockDevice.hpp>

int DummyBlockDevice::init()
{
    return 0;
}

int DummyBlockDevice::deinit()
{
    return 0;
}

int DummyBlockDevice::sync()
{
    return 0;
}

int DummyBlockDevice::read(void* buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int DummyBlockDevice::program(const void* buffer, bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int DummyBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

int DummyBlockDevice::trim(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t DummyBlockDevice::get_read_size() const
{
    return 0;
}

bd_size_t DummyBlockDevice::get_program_size() const
{
    return 0;
}

bd_size_t DummyBlockDevice::get_erase_size() const
{
    return 0;
}

bd_size_t DummyBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return 0;
}

int DummyBlockDevice::get_erase_value() const
{
    return 0;
}

bd_size_t DummyBlockDevice::size() const
{
    return 0;
}

void DummyBlockDevice::lock()
{
}

void DummyBlockDevice::unlock()
{
}
