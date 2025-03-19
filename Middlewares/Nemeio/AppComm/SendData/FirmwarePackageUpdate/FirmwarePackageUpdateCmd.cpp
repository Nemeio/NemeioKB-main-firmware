/*
 * FirmwarePackageUpdateCmd.cpp
 *
 *  Created on: Jan 20, 2021
 *      Author: thomas
 */

#include "FirmwarePackageUpdateCmd.hpp"

FirmwarePackageUpdateCmd::FirmwarePackageUpdateCmd(IFirmwareUpdate& firmwareUpdate)
    : SendDataListener(AppCommDefs::SendDataID::SENDDATAID_FIRMWARE_PACKAGE)
    , mFirmwareUpdate(firmwareUpdate)
{
}

uint8_t FirmwarePackageUpdateCmd::doPrepareSend(const uint8_t*, size_t, size_t dataLen)
{
    FirmwareUpdateDefs::ErrCode fwUpdateRet = mFirmwareUpdate.initUpdate(dataLen);

    return convertFirmwareUpdateErrCode(fwUpdateRet);
}

uint8_t FirmwarePackageUpdateCmd::doSendData(const uint8_t* pData, size_t len)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_UNEXPECTED;

    FirmwareUpdateDefs::ErrCode fwUpdateRet = mFirmwareUpdate.writeFirmware(pData, len);
    ret = convertFirmwareUpdateErrCode(fwUpdateRet);

    return ret;
}

uint8_t FirmwarePackageUpdateCmd::doTerminateSend()
{
    return AppCommDefs::APPCOMMERR_SUCCESS;
}

void FirmwarePackageUpdateCmd::doReset()
{
}

uint8_t FirmwarePackageUpdateCmd::doApplySend()
{
    FirmwareUpdateDefs::ErrCode fwUpdateRet = mFirmwareUpdate.runUpdate();

    return convertFirmwareUpdateErrCode(fwUpdateRet);
}

uint8_t FirmwarePackageUpdateCmd::convertFirmwareUpdateErrCode(
    FirmwareUpdateDefs::ErrCode fwUpdateErrCode) const
{
    uint8_t ret = AppCommDefs::APPCOMMERR_UNEXPECTED;

    switch(fwUpdateErrCode)
    {
    case FirmwareUpdateDefs::ErrCode::SUCCESS:
        ret = AppCommDefs::APPCOMMERR_SUCCESS;
        break;
    case FirmwareUpdateDefs::ErrCode::NOMEM:
        ret = AppCommDefs::APPCOMMERR_FWUPDATE_NOFLASH;
        break;
    case FirmwareUpdateDefs::ErrCode::WRITE:
        ret = AppCommDefs::APPCOMMERR_FWUPDATE_WRITE;
        break;
    case FirmwareUpdateDefs::ErrCode::STATE:
        ret = AppCommDefs::APPCOMMERR_UNEXPECTED;
        break;
    case FirmwareUpdateDefs::ErrCode::VERSION:
        ret = AppCommDefs::APPCOMMERR_FWUPDATE_VERSION;
        break;
    default:
        break;
    }

    return ret;
}
