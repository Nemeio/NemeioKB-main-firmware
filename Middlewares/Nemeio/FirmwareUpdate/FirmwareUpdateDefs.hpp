/*
 * FirmwareUpdateDefs.hpp
 *
 *  Created on: Feb 10, 2020
 *      Author: Labo
 */

#ifndef NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATEDEFS_HPP_
#define NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATEDEFS_HPP_

namespace FirmwareUpdateDefs
{
enum class ErrCode
{
    SUCCESS,
    NOMEM,
    WRITE,
    STATE,
    VERSION,
    BLE_CHIP_COMM_ERROR,
    UNEXPECTED
};
} // namespace FirmwareUpdateDefs

#endif /* NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATEDEFS_HPP_ */
