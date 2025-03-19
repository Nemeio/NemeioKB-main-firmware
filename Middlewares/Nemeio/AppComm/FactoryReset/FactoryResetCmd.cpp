/*
 * FactoryResetCmd.cpp
 *
 *  Created on: Aug 26, 2019
 *      Author: Labo
 */

#include "FactoryResetCmd.hpp"

FactoryResetCmd::FactoryResetCmd(IAppCommTxListener& appCommTxListener,
                                 IFactoryReset& factoryReset,
                                 ISystem& system)
    : AppCommCmd(AppCommDefs::AppCommCmd::FACTORYRESET, appCommTxListener)
    , mFactoryReset(factoryReset)
    , mSystem(system)
{
}

void FactoryResetCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        replyRetCode(AppCommDefs::APPCOMMERR_MALFORMED_DATA);
    }
    else
    {
        mFactoryReset.run(
            [this](IFactoryReset::ErrCode factoryResetRet)
            {
                AppCommDefs::AppCommErrCode ret = AppCommDefs::APPCOMMERR_SUCCESS;

                if(IFactoryReset::ErrCode::BLE == factoryResetRet)
                {
                    ret = AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR;
                }

                // The reception task must be blocked until the reset.
                // We must wait the response packet has been sent to the desktop app

                // TODO Currently the architecture does not allow easily to identify
                // when the packet has been sent so we must use a delay. Make the
                // response sending synchronous to remove the call to delay.

                replyRetCode(ret);
                mSystem.delay(DelayMs(DELAY_BEFORE_SYSTEM_RESET_MS));
            });
    }
}


void FactoryResetCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
