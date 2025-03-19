/*
 * STM32FirmwareUpdate.hpp
 *
 *  Created on: May 17, 2019
 *      Author: Labo
 */

#ifndef FIRMWAREUPDATE_FIRMWAREUPDATE_HPP_
#define FIRMWAREUPDATE_FIRMWAREUPDATE_HPP_

#include <cstdint>
#include <cstddef>
#include "IUpdateListener.hpp"
#include "IFirmwareUpdate.hpp"
#include "IFlashInterface.hpp"
#include "ISFU.hpp"
#include "AbstractWatchdog.hpp"
#include "ISystem.hpp"
#include "FlashWriteBuffer.hpp"

class STM32FirmwareUpdate : public IFirmwareUpdate
{
public:
    STM32FirmwareUpdate(IFlashInterface& flashInterface, ISFU& SFU);
    ~STM32FirmwareUpdate() = default;

    /* IFirmwareUpdate methods */
    FirmwareUpdateDefs::ErrCode initUpdate(uint32_t len) override;
    FirmwareUpdateDefs::ErrCode writeFirmware(const uint8_t* pData, std::size_t len) override;
    FirmwareUpdateDefs::ErrCode runUpdate() override;

private:
    IFlashInterface& mFlashInterface;
    ISFU& mSFU;

    FlashWriteBuffer<uint8_t, sizeof(uint64_t)> mFlashBuffer;
    uint8_t* mFlashDestBegin;
    uint8_t* mFlashDest;
    std::size_t mUpgradeLength = 0;

    void resetFlashAddr();
    FirmwareUpdateDefs::ErrCode writeFlash(const uint8_t* pSrc, std::size_t len);

    std::size_t _bufferFull(const uint8_t* data, size_t size);
};

#endif /* FIRMWAREUPDATE_FIRMWAREUPDATE_HPP_ */
