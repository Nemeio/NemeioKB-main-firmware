/*
 * SerialNumberCmd.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: Labo
 */

#include "SerialNumber/SerialNumberCmd.hpp"
#include "assertTools.h"

SerialNumberCmd::SerialNumberCmd(IAppCommTxListener& appCommTxListener,
                                 const ISerialNumber& serialNumber)
    : AppCommCmd(AppCommDefs::AppCommCmd::SERIALNUMBER, appCommTxListener)
    , mSerialNumber(serialNumber)
{
}

void SerialNumberCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        return;
    }

    char serialNumber[SERIAL_NUMBER_MAX_SIZE] = {0};
    etl::copy(mSerialNumber.getString().begin(), mSerialNumber.getString().end(), serialNumber);

    transmitResponse(reinterpret_cast<uint8_t*>(serialNumber), SERIAL_NUMBER_MAX_SIZE);
}
