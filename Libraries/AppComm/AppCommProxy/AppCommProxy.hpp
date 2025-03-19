/*
 * AppCommProxy.hpp
 *
 *  Created on: Jun 20, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_APPCOMMPROXY_APPCOMMPROXY_HPP_
#define APPCOMM_APPCOMMPROXY_APPCOMMPROXY_HPP_

#include "IConnectivityListener.hpp"
#include "IDataTransportRxListener.hpp"
#include "IAppCommTxListener/IAppCommTxListener.hpp"
#include "AppCommCmd.hpp"
#include "USBSerial.hpp"
#include "BleSerial.hpp"
#include "AppComm.hpp"
#include "cmsis_os.h"
#include <functional>
#include "Mutex.hpp"
#include "IAppCommCmd.hpp"

class AppCommProxy : public IConnectivityListener,
                     public IAppCommTxListener,
                     public IDataTransportRxListener
{
public:
    AppCommProxy(USBSerial& mUSBSerial, BleSerial& mBleSerial, ICRC32& crc32);
    virtual ~AppCommProxy() = default;

    void init();
    void registerRxListener(IAppCommCmd& listener);

    void onConnectivityChangeUsb() final;
    void onConnectivityChangeBle() final;
    void onConnectivityChangeNone() final;

    void sendCmd(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len);
    void sendResponse(AppCommDefs::AppCommCmd cmd, const uint8_t* pData, std::size_t len);

    void dataReceived(Identifiable& dataSource, const uint8_t* pData, std::size_t len);

private:
    AppComm mUSBAppComm;
    AppComm mBLEAppComm;

    std::reference_wrapper<AppComm> mCurrentAppCommRef;

    Mutex mAppCommLock;
};

#endif /* APPCOMM_APPCOMMPROXY_APPCOMMPROXY_HPP_ */
