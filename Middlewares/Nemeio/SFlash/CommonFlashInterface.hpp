/*
 * CommonFlashInterface.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_COMMONFLASHINTERFACE_HPP_
#define NEMEIO_SFLASH_COMMONFLASHINTERFACE_HPP_

enum class OpCode : uint8_t
{
    WRITE_REGISTER = 0x01,
    PAGE_PROGRAM_PP = 0x02,
    READ_DATA_BYTES_READ = 0x03,
    READ_STATUS_REGISTER_RDSR = 0x05,
    READ_CONFIG_REGISTER_RDCR = 0x15,
    WRITE_ENABLE_WREN = 0x06,
    READ_HIGHER_SPEED_FAST_READ = 0x0B,
    SECTOR_ERASE_SE = 0x20,
    QUAD_PAGE_PROG_QPP = 0x32,
    READ_UNIQUE_ID = 0x4B,
    QUAD_O_READ_QOR = 0x6B,

    /* 4 Bytes addressing */
    QUAD_O_READ_QOR_4B = 0x6C,
    QUAD_PAGE_PROG_QPP_4B = 0x34,
    SECTOR_ERASE_SE_4B = 0x21,

    READ_MANUFACTURER_AND_DEVICE_IDENTIFICATION = 0x9F,
};

// Write In Progress Status
static constexpr uint8_t SR1_WIP_IDX = 0;
static constexpr uint8_t SR1_WIP_DISABLED = 0;

// Write Enable Latch Status
static constexpr uint8_t SR1_WEL_IDX = 1;
static constexpr uint8_t SR1_WEL_ENABLED = 1;

// For address
static constexpr uint32_t MAX_VALUE_24_BITS = (1 << 24);

#endif /* NEMEIO_SFLASH_COMMONFLASHINTERFACE_HPP_ */
