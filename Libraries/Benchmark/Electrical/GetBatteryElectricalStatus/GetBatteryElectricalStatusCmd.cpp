/*
 * GetBatteryElectricalStatusCmd.cpp
 *
 *  Created on: 10 novembre 2022
 *      Author: Olivier
 */

#include "GetBatteryElectricalStatusCmd.hpp"

using namespace benchmark;

GetBatteryElectricalStatusCmd::GetBatteryElectricalStatusCmd(IAppCommTxListener& appCommTxListener,
                                                             AssemblyHardware& hardwareAssembly)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_ELECTRICAL_GETBATTERY_STATUS, appCommTxListener)
    , mHardwareAssembly(hardwareAssembly)
{
}

void GetBatteryElectricalStatusCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        replyRetCode(AppCommDefs::APPCOMMERR_MALFORMED_DATA);
    }
    else
    {
        GetBatteryProperties getBatteryProperties;
        getBatteryProperties.current = mHardwareAssembly.getFuelGauge().getCurrentMa();
        getBatteryProperties.voltage = mHardwareAssembly.getFuelGauge().getVoltageMv();

        getBatteryProperties.retCode = static_cast<int8_t>(AppCommDefs::APPCOMMERR_SUCCESS);
        transmitResponse(reinterpret_cast<uint8_t*>(&getBatteryProperties),
                         sizeof(getBatteryProperties));
    }
}

void GetBatteryElectricalStatusCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
