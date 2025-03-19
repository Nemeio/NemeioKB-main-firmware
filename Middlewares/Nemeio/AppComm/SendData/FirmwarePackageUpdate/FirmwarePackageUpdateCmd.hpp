/*
 * FirmwarePackageUpdateCmd.hpp
 *
 *  Created on: Jan 20, 2021
 *      Author: thomas
 */

#ifndef APPCOMM_SENDDATA_FirmwarePackageUpdateCmd_HPP_
#define APPCOMM_SENDDATA_FirmwarePackageUpdateCmd_HPP_

#include "SendDataListener.hpp"

#include "IFirmwareUpdate.hpp"

class FirmwarePackageUpdateCmd : public SendDataListener
{
public:
    FirmwarePackageUpdateCmd(IFirmwareUpdate& firmwareUpdate);
    virtual ~FirmwarePackageUpdateCmd() = default;

private:
    IFirmwareUpdate& mFirmwareUpdate;

    uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen);
    uint8_t doSendData(const uint8_t* pData, size_t len);
    uint8_t doTerminateSend();
    uint8_t doApplySend();
    void doReset();

    [[nodiscard]] uint8_t convertFirmwareUpdateErrCode(
        FirmwareUpdateDefs::ErrCode fwUpdateErrCode) const;
};

#endif /* APPCOMM_SENDDATA_FirmwarePackageUpdateCmd_HPP_ */
