/*
 * AppCommProxy.cpp
 *
 *  Created on: Jun 20, 2019
 *      Author: Labo
 */

#include "AppCommProxy.hpp"
#include "assertTools.h"
#include "AutoLock.h"

AppCommProxy::AppCommProxy(USBSerial& mUSBSerial, BleSerial& mBleSerial, ICRC32& crc32)
    : mUSBAppComm(mUSBSerial, crc32)
    , mBLEAppComm(mBleSerial, crc32)
    , mCurrentAppCommRef(std::ref(mUSBAppComm))
{
}

void AppCommProxy::sendCmd(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len)
{
    AutoLock autoLock(mAppCommLock);

    mCurrentAppCommRef.get().sendCmd(cmd, pData, len);
}

void AppCommProxy::sendResponse(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len)
{
    AutoLock autoLock(mAppCommLock);

    mCurrentAppCommRef.get().sendResponse(cmd, pData, len);
}

void AppCommProxy::dataReceived(Identifiable& dataSource, const uint8_t* pData, std::size_t len)
{
    AutoLock autoLock(mAppCommLock);

    if(mCurrentAppCommRef.get().isSameDataSource(dataSource))
    {
        mCurrentAppCommRef.get().dataReceived(dataSource, pData, len);
    }
}

void AppCommProxy::registerRxListener(IAppCommCmd& listener)
{
    mUSBAppComm.registerRxListener(listener);
    mBLEAppComm.registerRxListener(listener);
}

void AppCommProxy::init()
{
    mUSBAppComm.init();
    mBLEAppComm.init();
}

void AppCommProxy::onConnectivityChangeUsb()
{
    AutoLock autoLock(mAppCommLock);

    mCurrentAppCommRef = std::ref(mUSBAppComm);
}

void AppCommProxy::onConnectivityChangeBle()
{
    AutoLock autoLock(mAppCommLock);

    mCurrentAppCommRef = std::ref(mBLEAppComm);
}

void AppCommProxy::onConnectivityChangeNone()
{
    AutoLock autoLock(mAppCommLock);

    mCurrentAppCommRef = std::ref(mUSBAppComm);
}
