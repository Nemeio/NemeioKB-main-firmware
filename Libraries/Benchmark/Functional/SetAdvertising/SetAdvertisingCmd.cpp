#include "SetAdvertisingCmd.hpp"
#include "etl/string.h"

using namespace benchmark;

SetAdvertisingCmd::SetAdvertisingCmd(IAppCommTxListener& appCommTxListener,
                                     BleChip& bleChip,
                                     DeviceProperties& deviceProperties)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_FUNCTIONAL_SETADVERTISING, appCommTxListener)
    , m_bleChip(bleChip)
    , m_deviceProperties(deviceProperties)
{
}

void SetAdvertisingCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(bool enable; (sizeof(enable) == dataLen) && (pData != nullptr))
    {
        enable = *pData != 0 ? true : false;

        bool success = false;

        if(enable)
        {
            success = enableBle();
        }
        else
        {
            success = disableBle();
        }

        retCode = success ? AppCommDefs::APPCOMMERR_SUCCESS
                          : AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR;
    }

    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}

void SetAdvertisingCmd::notification(BleEvent event)
{
    if(event == BleEvent::STARTED)
    {
        m_bleStartedSignal.set();
    }
}


bool SetAdvertisingCmd::enableBle()
{
    m_bleChip.add_observer(*this);

    m_bleChip.powerOff();
    m_bleChip.init();

    bool ret = m_bleStartedSignal.wait(DelayMs::INFINITE_DELAY);

    m_bleChip.remove_observer(*this);

    if(ret)
    {
        configureBle();
        m_bleChip.enableBle();
    }

    return ret;
}

bool SetAdvertisingCmd::disableBle()
{
    m_bleChip.disableBle();

    return true;
}

void SetAdvertisingCmd::configureBle()
{
    etl::string<BLEDefs::DEVICE_NAME_MAX_SIZE> deviceName;
    deviceName.assign("NemeioTB_");
    deviceName.append(m_deviceProperties.getSerialNumber().getString().data());

    if(m_deviceProperties.isProvisioned())
    {
        m_bleChip.setDeviceMacAddress(m_deviceProperties.getBleMacAddress());
    }
    m_bleChip.setDeviceName(deviceName);

    m_bleChip.configure();
}