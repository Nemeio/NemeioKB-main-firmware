/*
 * Flash.cpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#include <Flash.hpp>
#include "TechnicalErrorDefs.hpp"
#include "TraceLogger.hpp"
#include "FlashTechErrorCode.hpp"

#define MODULE "flash"

Flash::Flash(const FlashInfo& flashInfo,
             ISFlashCommunicationInterface& flashCommunication,
             const FlashIOLut& flashInfos)
    : mFlashInfo(flashInfo)
    , mFlashConfiguration(flashCommunication)
    , mFlashIO(mFlashConfiguration, flashCommunication, flashInfos)
{
    flashCommunication.init(mFlashInfo.mTotalSize);
}
/* IFlashIO */
bool Flash::read(void* buffer, uint32_t addr, size_t size)
{
    bool ret = mFlashIO.read(buffer, addr, size);

    if(!ret)
    {
        LOG_ERR(MODULE,
                "Read error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                    static_cast<uint16_t>(
                                                                        FlashTechErrorCode::UNKNOWN))
                              << " on " << etl::hex << etl::showbase << this);
    }
    return ret;
}

bool Flash::writePage(uint32_t addr, size_t size, uint8_t* data)
{
    bool ret = mFlashIO.writePage(addr, size, data);
    if(!ret)
    {
        LOG_ERR(MODULE,
                "Write error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                     static_cast<uint16_t>(
                                                                         FlashTechErrorCode::UNKNOWN))
                               << " on " << etl::hex << etl::showbase << this);
    }
    return ret;
}

bool Flash::sectorErase(uint32_t addr)
{
    bool ret = mFlashIO.sectorErase(addr);
    if(!ret)
    {
        LOG_ERR(MODULE,
                "Erase error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                     static_cast<uint16_t>(
                                                                         FlashTechErrorCode::UNKNOWN))
                               << " on " << etl::hex << etl::showbase << this);
    }
    return ret;
}

const FlashInfo Flash::getFlashInfo()
{
    return mFlashInfo;
}

/* IFlashConfiguration */
bool Flash::readStatusRegisterCmd(uint8_t* data, size_t size)
{
    bool ret = mFlashConfiguration.readStatusRegisterCmd(data, size);
    if(!ret)
    {
        LOG_ERR(MODULE,
                "Read SR error " << TechnicalErrorDefs::TechnicalError(
                    getModuleId(), static_cast<uint16_t>(FlashTechErrorCode::UNKNOWN))
                                 << " on " << etl::hex << etl::showbase << this);
    }
    return ret;
}

bool Flash::readConfigurationRegisterCmd(uint8_t* data, size_t size)
{
    bool ret = mFlashConfiguration.readConfigurationRegisterCmd(data, size);
    if(!ret)
    {
        LOG_ERR(MODULE,
                "Read CR error " << TechnicalErrorDefs::TechnicalError(
                    getModuleId(), static_cast<uint16_t>(FlashTechErrorCode::UNKNOWN))
                                 << " on " << etl::hex << etl::showbase << this);
    }
    return ret;
}

bool Flash::writeRegistersCmd(uint8_t* data, size_t size)
{
    bool ret = mFlashConfiguration.writeRegistersCmd(data, size);
    if(!ret)
    {
        LOG_ERR(MODULE,
                "Write reg error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(
                                                              FlashTechErrorCode::UNKNOWN))
                    << " on " << etl::hex << etl::showbase << this);
    }
    return ret;
}

uint64_t Flash::getUniqueId()
{
    return mFlashConfiguration.getUniqueId();
}

uint32_t Flash::getIdentifier()
{
    return mFlashConfiguration.getIdentifier();
}

bool Flash::waitMemReady()
{
    return mFlashConfiguration.waitMemReady();
}

bool Flash::writeEnable()
{
    return mFlashConfiguration.writeEnable();
}
