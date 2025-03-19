/*
 * IFirmwareUpdate.hpp
 *
 *  Created on: Feb 12, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_FIRMWAREUPDATE_IFIRMWAREUPDATE_HPP_
#define NEMEIO_FIRMWAREUPDATE_IFIRMWAREUPDATE_HPP_

#include "FirmwareUpdateDefs.hpp"
#include <cstddef>
#include <cstdint>

class IFirmwareUpdate
{
public:
    IFirmwareUpdate() = default;
    virtual ~IFirmwareUpdate() = default;

    virtual FirmwareUpdateDefs::ErrCode initUpdate(uint32_t len) = 0;
    virtual FirmwareUpdateDefs::ErrCode writeFirmware(const uint8_t* pData, std::size_t len) = 0;
    virtual FirmwareUpdateDefs::ErrCode runUpdate() = 0;
};

#endif /* NEMEIO_FIRMWAREUPDATE_IFIRMWAREUPDATE_HPP_ */
