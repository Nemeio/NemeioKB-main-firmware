/*
 * IT8951FirmwareUpdate.hpp
 *
 *  Created on: 28 juil. 2020
 *      Author: eskoric
 */

#ifndef FIRMWAREUPDATE_IT8951FIRMWAREUPADTE_HPP_
#define FIRMWAREUPDATE_IT8951FIRMWAREUPADTE_HPP_

#include <IBlockDevice.hpp>

#include "IDataReader.hpp"
#include "ScreenDefs.hpp"
#include "IFirmwareUpdate.hpp"
#include "IUpdateListener.hpp"
#include "FlashWriteBuffer.hpp"

class IT8951FirmwareUpdate : public IFirmwareUpdate
{
public:
    explicit IT8951FirmwareUpdate(IBlockDevice& blockDevice);

    FirmwareUpdateDefs::ErrCode initUpdate(uint32_t len) override;
    FirmwareUpdateDefs::ErrCode writeFirmware(const uint8_t* pData, std::size_t len) override;
    FirmwareUpdateDefs::ErrCode runUpdate() override;

private:
    static constexpr bd_addr_t FIRMWARE_FLASH_ADDR = 0;
    static constexpr bd_addr_t FIRMWARE_FLASH_WRITE_SIZE = 256;

    IBlockDevice& mBlockDevice;

    bool mbInitialized = false;
    bd_addr_t mFlashDest = FIRMWARE_FLASH_ADDR;
    std::size_t mUpgradeLength = 0;
    FlashWriteBuffer<uint8_t, FIRMWARE_FLASH_WRITE_SIZE> mFlashBuffer;

    std::size_t _bufferFull(const uint8_t* data, size_t size);
};

#endif /* FIRMWAREUPDATE_IT8951FIRMWAREUPADTE_HPP_ */
