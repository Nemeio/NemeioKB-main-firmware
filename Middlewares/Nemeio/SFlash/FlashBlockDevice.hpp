/*
 * QSPIFlashBlockDevice.hpp
 *
 *  Created on: 3 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_FLASHBLOCKDEVICE_HPP_
#define SFLASH_FLASHBLOCKDEVICE_HPP_

#include <IBlockDevice.hpp>
#include <IFlash.hpp>
#include "RecursiveMutex.hpp"

class FlashBlockDevice : public IBlockDevice
{
public:
    explicit FlashBlockDevice(IFlashIO& flash);
    virtual ~FlashBlockDevice() = default;

    int init();
    int deinit();

    int read(void* buffer, bd_addr_t addr, bd_size_t size) final;
    int program(const void* buffer, bd_addr_t addr, bd_size_t size) final;
    int erase(bd_addr_t addr, bd_size_t size) final;
    bd_size_t get_read_size() const final;
    bd_size_t get_program_size() const final;
    bd_size_t get_erase_size() const final;
    bd_size_t get_erase_size(bd_addr_t addr) const final;
    bd_size_t size() const final;
    void lock() final;
    void unlock() final;

private:
    static constexpr uint32_t MUTEX_FLASH_OPERATION_TIMEOUT_MS = 30000;

    IFlashIO& mFlash;
    RecursiveMutex mMutex;
};

#endif /* SFLASH_FLASHBLOCKDEVICE_HPP_ */
