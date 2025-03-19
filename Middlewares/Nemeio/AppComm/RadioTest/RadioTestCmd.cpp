/*
 * RadioTestCmd.cpp
 *
 *  Created on: 22 nov. 2021
 *      Author: eskoric
 */

#include "RadioTestCmd.hpp"
RadioTestCmd::RadioTestCmd(IAppCommTxListener& appCommTxListener, IRadioTest& radioTest)
    : AppCommCmd(AppCommDefs::AppCommCmd::RADIO_TEST, appCommTxListener)
    , m_radioTest(radioTest)
{
}

void RadioTestCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != sizeof(BLEDefs::TestMode))
    {
        replyRetCode(AppCommDefs::APPCOMMERR_MALFORMED_DATA);
        return;
    }

    BLEDefs::TestMode mode = *reinterpret_cast<const BLEDefs::TestMode*>(pData);

    AppCommDefs::AppCommErrCode ret = m_radioTest.runMode(mode)
                                          ? AppCommDefs::APPCOMMERR_SUCCESS
                                          : AppCommDefs::APPCOMMERR_INVALID_CONTENT;

    replyRetCode(ret);
}

void RadioTestCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
