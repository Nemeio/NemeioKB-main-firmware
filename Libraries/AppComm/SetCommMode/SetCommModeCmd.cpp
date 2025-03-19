/*
 * SetCommModeCmd.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Labo
 */

#include <SetCommMode/SetCommModeCmd.hpp>

SetCommModeCmd::SetCommModeCmd(IAppCommTxListener& appCommTxListener,
                               KeyProtocolManager& keyProtocolManager)
    : AppCommCmd(AppCommDefs::AppCommCmd::SETCOMMMODE, appCommTxListener)
    , mKeyProtocolManager(keyProtocolManager)
{
}

void SetCommModeCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(sizeof(uint8_t) == dataLen)
    {
        keyprotocoldefs::KeyProtocolMode protocolMode =
            static_cast<keyprotocoldefs::KeyProtocolMode>(pData[0]);
        if(mKeyProtocolManager.setProtocolMode(protocolMode))
        {
            retCode = AppCommDefs::APPCOMMERR_SUCCESS;
        }
        else
        {
            retCode = AppCommDefs::APPCOMMERR_INVALID_CONTENT;
        }
    }

    replyRetCode(retCode);
}

void SetCommModeCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
