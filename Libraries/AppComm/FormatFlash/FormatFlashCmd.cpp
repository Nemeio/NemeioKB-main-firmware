/*
 * FormatFlashCmd.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: Labo
 */

#include "FormatFlashCmd.hpp"

FormatFlashCmd::FormatFlashCmd(IAppCommTxListener& appCommTxListener, IApplication& app)
    : AppCommCmd(AppCommDefs::AppCommCmd::FORMATFLASH, appCommTxListener)
    , mApp(app)
{
}

void FormatFlashCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        return;
    }

    mApp.FormatFlash();
}
