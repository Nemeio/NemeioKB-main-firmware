/*
 * STM32FirmwareUpdate.cpp
 *
 *  Created on: May 17, 2019
 *      Author: Labo
 */

#include "se_def.h"
#include "assertTools.h"
#include <string.h>
#include "STM32FirmwareUpdateRollback.hpp"

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdateRollback::initUpdate(uint32_t len)
{
    return FirmwareUpdateDefs::ErrCode::SUCCESS;
}

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdateRollback::writeFirmware(const uint8_t* pData,
                                                                       std::size_t len)
{
    return FirmwareUpdateDefs::ErrCode::SUCCESS;
}

FirmwareUpdateDefs::ErrCode STM32FirmwareUpdateRollback::runUpdate()
{
    // Nothing to do here
    // The new application has not been validated and application will reset the system after this call,
    // so SBSFU will rollback to the previous version
    return FirmwareUpdateDefs::ErrCode::SUCCESS;
}