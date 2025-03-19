/*
 * QSPIBlockDevice.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: Labo
 */

#ifndef FILESYSTEM_LINUX_BLOCK_DEVICE_HPP_
#define FILESYSTEM_LINUX_BLOCK_DEVICE_HPP_

#include <stdio.h>
#include <mutex>
#include <string_view>
#include "IBlockDevice.hpp"

#define FLASH_ADDR_NB_BITS 24
#define FLASH_SIZE_IN_BYTES (1 << FLASH_ADDR_NB_BITS)

#define PAGE_SIZE_IN_BYTES 256
#define SECTOR_SIZE_IN_BYTES (4 * 1024)
#define BLOCK_SIZE_IN_BYTES (64 * 1024)
#define FLASH_NUMBER_OF_SECTORS (FLASH_SIZE_IN_BYTES / SECTOR_SIZE_IN_BYTES)

class LinuxBlockDevice : public IBlockDevice
{
public:
    LinuxBlockDevice() = delete;
    LinuxBlockDevice(std::string_view file);
    virtual ~LinuxBlockDevice() = default;

    /** Initialize a block device
	 *
	 *  @return         0 on success or a negative error code on failure
	 */
    virtual int init();

    /** Deinitialize a block device
	 *
	 *  @return         0 on success or a negative error code on failure
	 */
    virtual int deinit();

    /** Read blocks from a block device
	 *
	 *  @param buffer   Buffer to write blocks to
	 *  @param addr     Address of block to begin reading from
	 *  @param size     Size to read in bytes, must be a multiple of read block size
	 *  @return         0 on success, negative error code on failure
	 */
    virtual int read(void* buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
	 *
	 *  The blocks must have been erased prior to being programmed
	 *
	 *  @param buffer   Buffer of data to write to blocks
	 *  @param addr     Address of block to begin writing to
	 *  @param size     Size to write in bytes, must be a multiple of program block size
	 *  @return         0 on success, negative error code on failure
	 */
    virtual int program(const void* buffer, bd_addr_t addr, bd_size_t size);

    /** Erase blocks on a block device
	 *
	 *  The state of an erased block is undefined until it has been programmed
	 *
	 *  @param addr     Address of block to begin erasing
	 *  @param size     Size to erase in bytes, must be a multiple of erase block size
	 *  @return         0 on success, negative error code on failure
	 */
    virtual int erase(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
	 *
	 *  @return         Size of a readable block in bytes
	 */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programable block
	 *
	 *  @return         Size of a programable block in bytes
	 *  @note Must be a multiple of the read size
	 */
    virtual bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
	 *
	 *  @return         Size of a eraseable block in bytes
	 *  @note Must be a multiple of the program size
	 */
    virtual bd_size_t get_erase_size() const;

    /** Get the size of an erasable block given address
	 *
	 *  @param addr     Address within the erasable block
	 *  @return         Size of an erasable block in bytes
	 *  @note Must be a multiple of the program size
	 */
    virtual bd_size_t get_erase_size(bd_addr_t addr) const;

    /** Get the value of storage when erased
	 *
	 *  If get_erase_value returns a non-negative byte value, the underlying
	 *  storage is set to that value when erased, and storage containing
	 *  that value can be programmed without another erase.
	 *
	 *  @return         The value of storage when erased, or -1 if you can't
	 *                  rely on the value of erased storage
	 */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
	 *
	 *  @return         Size of the underlying device in bytes
	 */
    virtual bd_size_t size() const;

    /** Lock the block device
	 *
	 *
	 */
    void lock();

    /** Unlock the block device
	 *
	 *
	 */
    void unlock();

private:
    static constexpr bd_size_t sReadSize = PAGE_SIZE_IN_BYTES;
    static constexpr bd_size_t sProgSize = PAGE_SIZE_IN_BYTES;
    static constexpr bd_size_t sEraseSize = SECTOR_SIZE_IN_BYTES;
    static constexpr bd_size_t sSize = FLASH_NUMBER_OF_SECTORS * SECTOR_SIZE_IN_BYTES;

    FILE* mFile;
    uint8_t mEraseBuffer[SECTOR_SIZE_IN_BYTES];
    std::mutex mMutex;
};

#endif /* FILESYSTEM_LINUX_BLOCK_DEVICE_HPP_ */
