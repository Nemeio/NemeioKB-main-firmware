/*
 * STM32FirmwareUpdate.hpp
 *
 *  Created on: May 17, 2019
 *      Author: Labo
 */

#ifndef FIRMWAREUPDATE_FIRMWAREUPDATEROLLBACK_HPP_
#define FIRMWAREUPDATE_FIRMWAREUPDATEROLLBACK_HPP_

#include <cstdint>
#include <cstddef>
#include "IFirmwareUpdate.hpp"
#include "ISFU.hpp"
#include "FlashWriteBuffer.hpp"

class STM32FirmwareUpdateRollback : public IFirmwareUpdate
{
public:
    STM32FirmwareUpdateRollback() = default;
    virtual ~STM32FirmwareUpdateRollback() = default;

    /* IFirmwareUpdate methods */
    FirmwareUpdateDefs::ErrCode initUpdate(uint32_t len) override;
    FirmwareUpdateDefs::ErrCode writeFirmware(const uint8_t* pData, std::size_t len) override;
    FirmwareUpdateDefs::ErrCode runUpdate() override;
};

#endif /* FIRMWAREUPDATE_FIRMWAREUPDATEROLLBACK_HPP_ */
