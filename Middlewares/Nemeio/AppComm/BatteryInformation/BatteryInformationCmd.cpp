/*
 * BatteryInformationCmd.cpp
 *
 *  Created on: Apr 26, 2019
 *      Author: Labo
 */

#include "../BatteryInformation/BatteryInformationCmd.hpp"

BatteryInformationCmd::BatteryInformationCmd(IAppCommTxListener& appCommTxListener,
                                             IBattery& battery)
    : AppCommCmd(AppCommDefs::AppCommCmd::BATTERYINFORMATION, appCommTxListener)
    , mBattery(battery)
{
}

void BatteryInformationCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    RespBuffer respBuffer;
    if(dataLen != 0)
    {
        setRetCodeResponse(AppCommDefs::APPCOMMERR_MALFORMED_DATA, respBuffer);
    }
    else
    {
        getBatteryInformation(respBuffer);
    }

    transmitResponse(respBuffer.data(), respBuffer.usedSpace());
}

void BatteryInformationCmd::setRetCodeResponse(AppCommDefs::AppCommErrCode ret,
                                               RespBuffer& respBuffer)
{
    RetCodeBatteryInformationResponse response(ret);
    respBuffer.pushBack(response);
}

void BatteryInformationCmd::getBatteryInformation(RespBuffer& respBuffer)
{
    BatteryInformation batteryInformation;
    IBattery::RetCode batteryRetCode = mBattery.getBatteryInformation(batteryInformation);

    AppCommDefs::AppCommErrCode appCommErrCode = convertBatteryRetCode(batteryRetCode);

    if(AppCommDefs::APPCOMMERR_SUCCESS == appCommErrCode)
    {
        setBatteryInformationResponse(appCommErrCode, batteryInformation, respBuffer);
    }
    else
    {
        setRetCodeResponse(appCommErrCode, respBuffer);
    }
}

void BatteryInformationCmd::setBatteryInformationResponse(
    AppCommDefs::AppCommErrCode ret,
    const BatteryInformation& batteryInformation,
    RespBuffer& respBuffer)
{
    BatteryInformationResponse batteryInformationResponse(ret, batteryInformation);
    respBuffer.pushBack(batteryInformationResponse);
}

AppCommDefs::AppCommErrCode BatteryInformationCmd::convertBatteryRetCode(
    IBattery::RetCode batteryRetCode)
{
    AppCommDefs::AppCommErrCode appCommErrCode = AppCommDefs::APPCOMMERR_UNEXPECTED;

    switch(batteryRetCode)
    {
    case IBattery::RetCode::SUCCESS:
        appCommErrCode = AppCommDefs::APPCOMMERR_SUCCESS;
        break;
    case IBattery::RetCode::INFORMATION_NOT_READY:
        appCommErrCode = AppCommDefs::APPCOMMERR_BATTERY_NOT_READY_ERROR;
        break;
    case IBattery::RetCode::FUELGAUGE_ERROR:
        appCommErrCode = AppCommDefs::APPCOMMERR_BATTERY_FUEL_GAUGE_ERROR;
        break;
    }

    return appCommErrCode;
}
