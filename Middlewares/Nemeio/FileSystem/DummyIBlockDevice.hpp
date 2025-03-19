/*
 * DummyBlockDevice.hpp
 *
 *  Created on: 9 déc. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FILESYSTEM_DUMMYIBLOCKDEVICE_HPP_
#define NEMEIO_FILESYSTEM_DUMMYIBLOCKDEVICE_HPP_

#include <IBlockDevice.hpp>

class DummyBlockDevice : public IBlockDevice
{
public:
    DummyBlockDevice() = default;
    virtual ~DummyBlockDevice() = default;

    int init() final;
    int deinit() final;
    int sync() final;
    int read(void* buffer, bd_addr_t addr, bd_size_t size) final;
    int program(const void* buffer, bd_addr_t addr, bd_size_t size) final;
    int erase(bd_addr_t addr, bd_size_t size) final;
    int trim(bd_addr_t addr, bd_size_t size) final;
    bd_size_t get_read_size() const final;
    bd_size_t get_program_size() const final;
    bd_size_t get_erase_size() const final;
    bd_size_t get_erase_size(bd_addr_t addr) const final;
    int get_erase_value() const final;
    bd_size_t size() const final;
    void lock() final;
    void unlock() final;
};

#endif /* NEMEIO_FILESYSTEM_DUMMYIBLOCKDEVICE_HPP_ */
