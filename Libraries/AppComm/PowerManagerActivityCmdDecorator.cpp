/*
 * PowerManagedCmd.cpp
 *
 *  Created on: 20 août 2021
 *      Author: thomas
 */

#include "PowerManagerActivityCmdDecorator.hpp"

PowerManagerActivityCmdDecorator::PowerManagerActivityCmdDecorator(IPowerManager& powerManager,
                                                                   AppCommCmd& cmd)
    : mAppCommCmd(cmd)
    , mPowerManager(powerManager)
{
}

void PowerManagerActivityCmdDecorator::dataReceived(const uint8_t* pData, size_t dataLen)
{
    mPowerManager.systemActivityHappened();
    mAppCommCmd.dataReceived(pData, dataLen);
}

AppCommDefs::AppCommCmd PowerManagerActivityCmdDecorator::getCmdId() const
{
    return mAppCommCmd.getCmdId();
}
