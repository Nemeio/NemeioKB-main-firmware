/*
 * SetFrontlightPowerCmd.cpp
 *
 *  Created on: 13 novembre 2022
 *      Author: Olivier
 */

#include "SetFrontlightPowerCmd.hpp"

using namespace benchmark;

SetFrontlightPowerCmd::SetFrontlightPowerCmd(IAppCommTxListener& appCommTxListener,
                                             AssemblyHardware& hardwareAssembly)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_COMMON_SETFRONTLIGHTPOWER, appCommTxListener)
    , mHardwareAssembly(hardwareAssembly)
{
}

void SetFrontlightPowerCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(uint8_t frontlightPower = 0; (sizeof(frontlightPower) == dataLen) && (pData != nullptr))
    {
        frontlightPower = *pData;

        if(frontlightPower > MAX_PERCENT_POWER)
        {
            retCode = AppCommDefs::APPCOMMERR_INVALID_CONTENT;
        }
        else
        {
            mHardwareAssembly.getLedController().setLightIntensity(frontlightPower);
            retCode = AppCommDefs::APPCOMMERR_SUCCESS;
        }
    }

    replyRetCode(retCode);
}

void SetFrontlightPowerCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
