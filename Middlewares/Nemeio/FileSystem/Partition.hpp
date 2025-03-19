/*
 * Partition.hpp
 *
 *  Created on: 25 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FILESYSTEM_PARTITION_HPP_
#define NEMEIO_FILESYSTEM_PARTITION_HPP_

#include <IBlockDevice.hpp>
#include <cstddef>
#include <cstdint>
#include "cpp_helper.hpp"

class Partition : public IBlockDevice
{
public:
    Partition(IBlockDevice& blockDevice, const bd_addr_t startAddress, const bd_size_t size);
    virtual ~Partition() = default;

    COPYABLE_MOVABLE(Partition, delete);

    virtual int init() override;
    virtual int deinit() override;
    virtual int read(void* buffer, bd_addr_t addr, bd_size_t size) override;
    virtual int program(const void* buffer, bd_addr_t addr, bd_size_t size) override;
    virtual int erase(bd_addr_t addr, bd_size_t size) override;
    virtual bd_size_t get_read_size() const override;
    virtual bd_size_t get_program_size() const override;
    virtual bd_size_t get_erase_size() const override;
    virtual bd_size_t get_erase_size(bd_addr_t addr) const override;
    virtual int get_erase_value() const override;
    virtual bd_size_t size() const override;

private:
    IBlockDevice& mBlockDevice;
    const bd_addr_t mOffset;
    const bd_size_t mSize;
};

#endif /* NEMEIO_FILESYSTEM_PARTITION_HPP_ */
