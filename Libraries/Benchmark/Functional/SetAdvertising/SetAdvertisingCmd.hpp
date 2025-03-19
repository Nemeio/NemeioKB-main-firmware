#pragma once
#include "AppCommCmd.hpp"
#include "BleChip.hpp"
#include "DeviceProperties.hpp"
#include "BleEventObserver.hpp"
#include "Signal.hpp"

namespace benchmark
{
class SetAdvertisingCmd : public AppCommCmd, public BleEventObserver
{
public:
    SetAdvertisingCmd(IAppCommTxListener& appCommTxListener,
                      BleChip& bleChip,
                      DeviceProperties& deviceProperties);
    ~SetAdvertisingCmd() final = default;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void notification(BleEvent event) final;

private:
    static constexpr uint32_t BLE_WAIT_START_TIMEOUT_MS = 2000;

    BleChip& m_bleChip;
    DeviceProperties& m_deviceProperties;
    Signal m_bleStartedSignal;

    bool enableBle();
    bool disableBle();
    void configureBle();
};

} // namespace benchmark
