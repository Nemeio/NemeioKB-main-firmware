#pragma once

#include "IBlockDevice.hpp"
#include "IFlashInterface.hpp"
#include "RecursiveMutex.hpp"

class InternalFlashBlockDevice : public IBlockDevice
{
public:
    explicit InternalFlashBlockDevice(IFlashInterface& flashInterface);
    ~InternalFlashBlockDevice() final = default;

    int init() final;
    int deinit() final;

    int read(void* buffer, bd_addr_t addr, bd_size_t size) final;
    int program(const void* buffer, bd_addr_t addr, bd_size_t size) final;
    int erase(bd_addr_t addr, bd_size_t size) final;
    bd_size_t get_read_size() const final;
    bd_size_t get_program_size() const final;
    bd_size_t get_erase_size() const final;
    bd_size_t get_erase_size(bd_addr_t addr) const final;
    bd_size_t size() const final;

private:
    static constexpr uint32_t MUTEX_FLASH_OPERATION_TIMEOUT_MS = 6000;

    IFlashInterface& m_flashInterface;
    RecursiveMutex m_mutex;
};