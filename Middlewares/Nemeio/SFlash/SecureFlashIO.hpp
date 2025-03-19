/*
 * SecureFlash.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_SECUREFLASHIO_HPP_
#define NEMEIO_SFLASH_SECUREFLASHIO_HPP_

#include "Flash.hpp"
#include "ISecureFlashIO.hpp"

class SecureFlashIO : public ISecureFlashIO
{
public:
    explicit SecureFlashIO(const FlashInfo& flashInfo,
                           ISFlashCommunicationInterface& flashCommunication,
                           const FlashIOLut& flashInfos);
    virtual ~SecureFlashIO() = default;

    bool readSecure(void* buffer, uint32_t addr, size_t size) override;
    bool writePageSecure(uint32_t addr, size_t size, uint8_t* data) override;
    bool sectorEraseSecure(uint32_t addr) override;

    const FlashInfo getFlashInfoSecure() final;

private:
    SFlashConfigurationCommonInterface mFlashConfiguration;
    SFlashIOCommonInterface mFlashIO;
    const FlashInfo mFlashInfo;
};

#endif /* NEMEIO_SFLASH_SECUREFLASHIO_HPP_ */
