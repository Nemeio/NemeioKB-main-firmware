/*
 * SystemResetCmd.cpp
 *
 *  Created on: Dec 17, 2019
 *      Author: Labo
 */

#include "SystemResetCmd.hpp"

SystemResetCmd::SystemResetCmd(IAppCommTxListener& appCommTxListener, ISystem& system)
    : AppCommCmd(AppCommDefs::AppCommCmd::SYSTEMRESET, appCommTxListener)
    , mSystem(system)
{
}

void SystemResetCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        replyRetCode(AppCommDefs::APPCOMMERR_MALFORMED_DATA);
        return;
    }

    mSystem.reset();
}

void SystemResetCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
