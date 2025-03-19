/*
 * SystemUpdate.cpp
 *
 *  Created on: 21 sept. 2021
 *      Author: thomas
 */

#include "SystemUpdate.hpp"

SystemUpdate::SystemUpdate(UpdateEngine& engine, ISystemUpdateFile& file)
    : mUpdateEngine(engine)
    , mSystemUpdateFile(file)
{
}

FirmwareUpdateDefs::ErrCode SystemUpdate::initUpdate(uint32_t len)
{
    return (mSystemUpdateFile.open() == fs::FSErrCode::FS_ERR_OK
                ? FirmwareUpdateDefs::ErrCode::SUCCESS
                : FirmwareUpdateDefs::ErrCode::UNEXPECTED);
}

FirmwareUpdateDefs::ErrCode SystemUpdate::writeFirmware(const uint8_t* pData, std::size_t len)
{
    return (mSystemUpdateFile.write(pData, len) == fs::FSErrCode::FS_ERR_OK
                ? FirmwareUpdateDefs::ErrCode::SUCCESS
                : FirmwareUpdateDefs::ErrCode::UNEXPECTED);
}

FirmwareUpdateDefs::ErrCode SystemUpdate::runUpdate()
{
    mSystemUpdateFile.close();

    return mUpdateEngine.run(mSystemUpdateFile, *this, *this);
}

bool SystemUpdate::registerListener(IUpdateListener& listener)
{
    mUpdateListener.push_back(listener);
    return true;
}

bool SystemUpdate::registerComponentUpdateListener(IUpdateStatusListener& listener)
{
    mUpdateStatusListener.push_back(listener);
    return true;
}


void SystemUpdate::notifyUpdateStatus(UpdateStatusDefs::UpdateStatus updateStatus)
{
    for(auto& listener: mUpdateStatusListener)
    {
        listener.get().notifyUpdateStatus(updateStatus);
    }
}

void SystemUpdate::onUpdateEvent(UpdateStatus status)
{
    for(auto& listener: mUpdateListener)
    {
        listener.get().onUpdateEvent(status);
    }
}
