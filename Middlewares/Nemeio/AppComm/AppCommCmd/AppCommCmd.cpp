/*
 * AppCommCmd.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: Labo
 */

#include "AppCommCmd.hpp"

AppCommCmd::AppCommCmd(AppCommDefs::AppCommCmd cmdId, IAppCommTxListener& appCommTxListener)
    : mCmdId(cmdId)
    , mAppCommTxListener(appCommTxListener)
{
}

void AppCommCmd::transmitCmd(const uint8_t* pData, size_t len)
{
    mAppCommTxListener.sendCmd(mCmdId, pData, len);
}

void AppCommCmd::transmitResponse(const uint8_t* pData, size_t len)
{
    mAppCommTxListener.sendResponse(mCmdId, pData, len);
}
