/*
 * Flash.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_FLASH_HPP_
#define NEMEIO_SFLASH_FLASH_HPP_

#include "IFlash.hpp"
#include "ISFlashCommunicationInterface.hpp"
#include "SFlashIOCommonInterface.hpp"
#include "SFlashConfigurationCommonInterface.hpp"
#include "Module.hpp"

class Flash : public IFlash, public Module<Identification::ModuleId::FLASH_MEMORY>
{
public:
    Flash(const FlashInfo& flashInfo,
          ISFlashCommunicationInterface& flashCommunication,
          const FlashIOLut& flashInfos);
    virtual ~Flash() = default;

    /* IFlashIO */
    bool read(void* buffer, uint32_t addr, size_t size) override;
    bool writePage(uint32_t addr, size_t size, uint8_t* data) override;
    bool sectorErase(uint32_t addr) override;

    const FlashInfo getFlashInfo() override;

    /* IFlashConfiguration */
    bool readStatusRegisterCmd(uint8_t* data, size_t size) override;
    bool readConfigurationRegisterCmd(uint8_t* data, size_t size) override;

    bool writeRegistersCmd(uint8_t* data, size_t size) override;

    uint64_t getUniqueId() override;
    uint32_t getIdentifier() override;

    bool waitMemReady() override;
    bool writeEnable() override;

private:
    const FlashInfo mFlashInfo;
    SFlashConfigurationCommonInterface mFlashConfiguration;
    SFlashIOCommonInterface mFlashIO;
};

#endif /* NEMEIO_SFLASH_FLASH_HPP_ */
