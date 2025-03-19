/*
 * BlockDeviceStrategy.hpp
 *
 *  Created on: 9 déc. 2021
 *      Author: thomas
 */

#ifndef ASSEMBLY_ASSEMBLYFILESYSTEM_IBLOCKDEVICESTRATEGY_HPP_
#define ASSEMBLY_ASSEMBLYFILESYSTEM_IBLOCKDEVICESTRATEGY_HPP_

#include <IBlockDevice.hpp>
#include <functional>

class BlockDeviceStrategy : public IBlockDevice
{
public:
    explicit BlockDeviceStrategy(IBlockDevice& blockDevice);
    ~BlockDeviceStrategy() = default;

    void setBlockDevice(IBlockDevice& blockDevice);

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

private:
    std::reference_wrapper<IBlockDevice> mBlockDevice;
};

#endif /* ASSEMBLY_ASSEMBLYFILESYSTEM_IBLOCKDEVICESTRATEGY_HPP_ */
