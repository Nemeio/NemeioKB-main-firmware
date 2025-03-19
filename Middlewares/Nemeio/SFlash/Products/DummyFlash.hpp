/*
 * DummyFlash.hpp
 *
 *  Created on: 6 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_DUMMYFLASH_HPP_
#define SFLASH_DUMMYFLASH_HPP_

#include <IFlash.hpp>

class DummyFlash : public IFlash
{
public:
    DummyFlash() = default;
    virtual ~DummyFlash() = default;

    bool init() final;
    bool deinit() final;

    bool read(void* buffer, uint32_t addr, size_t size) final;
    bool sectorErase(uint32_t addr) final;
    bool writePage(uint32_t addr, size_t size, uint8_t* data) final;

    const FlashInfo getFlashInfo() final;

    bool readStatusRegisterCmd(uint8_t* data, size_t size) final;
    bool readConfigurationRegisterCmd(uint8_t* data, size_t size) final;

    bool writeRegistersCmd(uint8_t* data, size_t size) final;

    uint64_t getUniqueId() final;
    uint32_t getIdentifier() final;

    bool waitMemReady() final;
    bool writeEnable() final;
};

#endif /* SFLASH_DUMMYFLASH_HPP_ */
