/*
 * KeyPressedCmd.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: Labo
 */

#include "KeyPressedCmd.hpp"

KeyPressedCmd::KeyPressedCmd(IAppCommTxListener& appCommTxListener)
    : AppCommCmd(AppCommDefs::AppCommCmd::KEYPRESS, appCommTxListener)
{
}

void KeyPressedCmd::sendKeyData(uint8_t* pData, size_t len)
{
    transmitCmd(pData, len);
}

void KeyPressedCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
}
