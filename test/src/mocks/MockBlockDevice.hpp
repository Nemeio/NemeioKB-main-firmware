#ifndef MOCKBLOCKDEVICE_HPP_
#define MOCKBLOCKDEVICE_HPP_

#include "gmock/gmock.h"
#include "IBlockDevice.hpp"

class MockBlockDevice : public IBlockDevice
{
public:
    MOCK_METHOD(int, init, (), (override));
    MOCK_METHOD(int, deinit, (), (override));
    MOCK_METHOD(int, sync, (), (override));
    MOCK_METHOD(int, read, (void* buffer, bd_addr_t addr, bd_size_t size), (override));
    MOCK_METHOD(int, program, (const void* buffer, bd_addr_t addr, bd_size_t size), (override));
    MOCK_METHOD(int, erase, (bd_addr_t addr, bd_size_t size), (override));
    MOCK_METHOD(int, trim, (bd_addr_t addr, bd_size_t size), (override));
    MOCK_METHOD(bd_size_t, get_read_size, (), (const, override));
    MOCK_METHOD(bd_size_t, get_program_size, (), (const, override));
    MOCK_METHOD(bd_size_t, get_erase_size, (), (const, override));
    MOCK_METHOD(int, get_erase_value, (), (const, override));
    MOCK_METHOD(bd_size_t, size, (), (const, override));
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
};

#endif /* MOCKBLOCKDEVICE_HPP_ */
