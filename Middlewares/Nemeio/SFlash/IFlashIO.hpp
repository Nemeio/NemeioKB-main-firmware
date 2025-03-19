/*
 * IFlash.hpp
 *
 *  Created on: 2 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_IFLASHIO_HPP_
#define SFLASH_IFLASHIO_HPP_

#include <cstdint>
#include <cstddef>

#include "FlashInfo.hpp"

class IFlashIO
{
public:
    IFlashIO() = default;
    virtual ~IFlashIO() = default;

    virtual bool read(void* buffer, uint32_t addr, size_t size) = 0;
    virtual bool writePage(uint32_t addr, size_t size, uint8_t* data) = 0;
    virtual bool sectorErase(uint32_t addr) = 0;

    virtual const FlashInfo getFlashInfo() = 0;
};


#endif /* SFLASH_IFLASHIO_HPP_ */
