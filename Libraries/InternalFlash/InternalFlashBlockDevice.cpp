#include "InternalFlashBlockDevice.hpp"
#include "assertTools.h"
#include "AutoLock.h"
#include "InternalFlashAddr.hpp"

InternalFlashBlockDevice::InternalFlashBlockDevice(IFlashInterface& flashInterface)
    : m_flashInterface(flashInterface)
{
}

int InternalFlashBlockDevice::init()
{
    return BD_ERROR_OK;
}

int InternalFlashBlockDevice::deinit()
{
    return BD_ERROR_OK;
}

int InternalFlashBlockDevice::read(void* buffer, bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_read(addr, size));
    ASSERT(m_flashInterface.isFlashZone(reinterpret_cast<void*>(addr), size));

    AutoLock lock(m_mutex, DelayMs(MUTEX_FLASH_OPERATION_TIMEOUT_MS));

    memcpy(buffer, reinterpret_cast<void*>(addr), size);

    return BD_ERROR_OK;
}

int InternalFlashBlockDevice::program(const void* buffer, bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_program(addr, size));
    ASSERT(m_flashInterface.isFlashZone(reinterpret_cast<void*>(addr), size));

    AutoLock lock(m_mutex, DelayMs(MUTEX_FLASH_OPERATION_TIMEOUT_MS));

    return m_flashInterface.write(reinterpret_cast<void*>(addr), buffer, size)
               ? BD_ERROR_OK
               : BD_ERROR_DEVICE_ERROR;
}

int InternalFlashBlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    ASSERT(is_valid_erase(addr, size));
    ASSERT(m_flashInterface.isFlashErasableZone(reinterpret_cast<void*>(addr), size));

    AutoLock lock(m_mutex, DelayMs(MUTEX_FLASH_OPERATION_TIMEOUT_MS));

    return m_flashInterface.eraseSize(reinterpret_cast<void*>(addr), size) ? BD_ERROR_OK
                                                                           : BD_ERROR_DEVICE_ERROR;
}

bd_size_t InternalFlashBlockDevice::get_read_size() const
{
    return 1;
}

bd_size_t InternalFlashBlockDevice::get_program_size() const
{
    return sizeof(uint64_t);
}

bd_size_t InternalFlashBlockDevice::get_erase_size() const
{
    return m_flashInterface.getPageSize();
}

bd_size_t InternalFlashBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return m_flashInterface.isFlashErasableZone(reinterpret_cast<void*>(addr), 0) ? get_erase_size()
                                                                                  : 0U;
}

bd_size_t InternalFlashBlockDevice::size() const
{
    // We use absolute addresses so addresses start from 0 to OTP area
    // Address will be checked for each read/program/erase operation
    // to make sure we are in a flash zone
    return reinterpret_cast<bd_addr_t>(m_flashInterface.getEnd()) + 1;
}
