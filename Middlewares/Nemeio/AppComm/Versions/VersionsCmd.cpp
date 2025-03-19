/*
 * VersionsCmd.cpp
 *
 *  Created on: May 20, 2019
 *      Author: Labo
 */

#include "VersionsCmd.hpp"
#include "utils.hpp"
#include "BleGetFwVersionCmd.hpp"

VersionsCmd::VersionsCmd(IAppCommTxListener& appCommTxListener,
                         IBleChip& bleChip,
                         IScreenVersionsReader& screenVersionsReader)
    : AppCommCmd(AppCommDefs::AppCommCmd::VERSIONS, appCommTxListener)
    , mBLEChip(bleChip)
    , mScreenVersionsReader(screenVersionsReader)
{
}

void VersionsCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        return;
    }

    if(mComponentVersions.errCodeCPUVersion != AppCommDefs::APPCOMMERR_SUCCESS)
    {
        getCPUVersion(mComponentVersions);
    }

    if(mComponentVersions.errCodeDisplayChipVersions != AppCommDefs::APPCOMMERR_SUCCESS)
    {
        getDisplayChipVersions(mComponentVersions);
    }

    /* If BLE version has NOT been already received, it will be sent by BLE version callback. Else send it directly */
    if(mComponentVersions.errCodeBLEVersion != AppCommDefs::APPCOMMERR_SUCCESS)
    {
        BleGetFwVersionCmd cmd;
        mSignal.clear();
        cmd.add_observer(*this);

        BLEDefs::ErrCode bleReqRet = mBLEChip.sendRequest(etl::reference_wrapper(cmd));

        if(bleReqRet == BLEDefs::ErrCode::SUCCESS)
        {
            mSignal.wait(DelayMs::INFINITE_DELAY);
        }
        else
        {
            mComponentVersions.errCodeBLEVersion = AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR;
        }
    }

    transmitVersionList(mComponentVersions);
}

void VersionsCmd::notification(const BleFwVersionEvent& event)
{
    mComponentVersions.errCodeBLEVersion = AppCommDefs::APPCOMMERR_SUCCESS;
    snprintf(mComponentVersions.bleVersion,
             sizeof(mComponentVersions.bleVersion),
             "%d.%d",
             event.version.major,
             event.version.minor);

    mSignal.set(0);
}

void VersionsCmd::notification(BleTxRequestTimeout event)
{
    mComponentVersions.errCodeBLEVersion = AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR;
    mSignal.set(0);
}

void VersionsCmd::getCPUVersion(VersionList& versionList)
{
    versionList.errCodeCPUVersion = AppCommDefs::APPCOMMERR_SUCCESS;
    snprintf(versionList.cpuVersion,
             sizeof(versionList.cpuVersion),
             "%d.%d",
             APP_MAJOR_NUMBER,
             APP_BUILD_NUMBER);
}

void VersionsCmd::getDisplayChipVersions(VersionList& versionList)
{
    ScreenDefs::ScreenErrCode screenRet =
        mScreenVersionsReader.getVersions(versionList.displayChipFirmwareVersion,
                                          sizeof(versionList.displayChipFirmwareVersion),
                                          versionList.displayChipLUTVersion,
                                          sizeof(versionList.displayChipLUTVersion));

    AppCommDefs::AppCommErrCode ret = AppCommDefs::APPCOMMERR_UNEXPECTED;

    switch(screenRet)
    {
    case ScreenDefs::ScreenErrCode::SUCCESS:
        ret = AppCommDefs::APPCOMMERR_SUCCESS;
        break;
    case ScreenDefs::ScreenErrCode::INVALID_STATE:
        ret = AppCommDefs::APPCOMMERR_DISPLAY_NOT_READY;
        break;
    default:
        break;
    }

    versionList.errCodeDisplayChipVersions = ret;
}

void VersionsCmd::transmitVersionList(const VersionList& versionList)
{
    transmitResponse(reinterpret_cast<const uint8_t*>(&versionList), sizeof(versionList));
}
