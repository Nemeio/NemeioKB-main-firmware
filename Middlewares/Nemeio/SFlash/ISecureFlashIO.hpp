/*
 * ISecureFlashIO.hpp
 *
 *  Created on: 5 janv. 2022
 *      Author: thomas
 */

#ifndef SFLASH_ISECUREFLASHIO_HPP_
#define SFLASH_ISECUREFLASHIO_HPP_

#include <cstdint>
#include "FlashInfo.hpp"

class ISecureFlashIO
{
public:
    virtual bool readSecure(void* buffer, uint32_t addr, size_t size) = 0;
    virtual bool writePageSecure(uint32_t addr, size_t size, uint8_t* data) = 0;
    virtual bool sectorEraseSecure(uint32_t addr) = 0;

    virtual const FlashInfo getFlashInfoSecure() = 0;
};

#endif /* SFLASH_ISECUREFLASHIO_HPP_ */
