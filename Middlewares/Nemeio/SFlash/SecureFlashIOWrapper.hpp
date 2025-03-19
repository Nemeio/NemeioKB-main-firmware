/*
 * SecureFlashWrapper.hpp
 *
 *  Created on: 3 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_SECUREFLASH_HPP_
#define SFLASH_SECUREFLASH_HPP_

#include "IFlash.hpp"
#include "ISecureFlashIO.hpp"

class SecureFlashIOWrapper : public IFlashIO
{
public:
    explicit SecureFlashIOWrapper(ISecureFlashIO& flash);
    virtual ~SecureFlashIOWrapper() = default;

    bool read(void* buffer, uint32_t addr, size_t size) final;
    bool writePage(uint32_t addr, size_t size, uint8_t* data) final;
    bool sectorErase(uint32_t addr) final;

    const FlashInfo getFlashInfo() final;

private:
    ISecureFlashIO& mSecureFlashIO;
};

#endif /* SFLASH_SECUREFLASH_HPP_ */
