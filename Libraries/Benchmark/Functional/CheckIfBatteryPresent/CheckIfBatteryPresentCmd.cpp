/*
 * CheckIfBatteryPresentCmd.cpp
 *
 *  Created on: 10 novembre 2022
 *      Author: Olivier
 */

#include "CheckIfBatteryPresentCmd.hpp"

using namespace benchmark;

CheckIfBatteryPresentCmd::CheckIfBatteryPresentCmd(IAppCommTxListener& appCommTxListener,
                                                   AssemblyHardware& hardwareAssembly)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_FUNCTIONAL_CHECKBATTERYPRESENT, appCommTxListener)
    , mHardwareAssembly(hardwareAssembly)
{
}

void CheckIfBatteryPresentCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        replyRetCode(AppCommDefs::APPCOMMERR_MALFORMED_DATA);
    }
    else
    {
        mHardwareAssembly.getBatteryCharger().disableCharge();
        mHardwareAssembly.getSystemItem().delay(1000);

        uint16_t voltage = mHardwareAssembly.getFuelGauge().getVoltageMv();

        mHardwareAssembly.getBatteryCharger().enableCharge();

        CheckBatteryPresenceProperties checkBatteryPresenceProperties;
        checkBatteryPresenceProperties.retCode = AppCommDefs::APPCOMMERR_SUCCESS;
        checkBatteryPresenceProperties.isBatteryPresent = (voltage
                                                           > MIN_BATTERY_VOLTAGE_DETECTION_MV);

        transmitResponse(reinterpret_cast<uint8_t*>(&checkBatteryPresenceProperties),
                         sizeof(checkBatteryPresenceProperties));
    }
}

void CheckIfBatteryPresentCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
