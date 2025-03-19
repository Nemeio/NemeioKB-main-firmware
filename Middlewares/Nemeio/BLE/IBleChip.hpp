#pragma once

#include "BlePublicTxCommands.hpp"
#include "BleEventObserver.hpp"
#include "etl/string_view.h"
#include "MacAddress.hpp"

inline constexpr size_t BLE_CHIP_NB_MAX_BLE_EVENT_LISTENERS = 4;

using BleEventObservable = ProtectedObservable<BleEventObserver, BLE_CHIP_NB_MAX_BLE_EVENT_LISTENERS>;

class IBleChip : public BleEventObservable
{
public:
    IBleChip() = default;
    virtual ~IBleChip() = default;

    virtual void registerBleCommandListener(AbstractBleCommandListener& commandListener) = 0;
    virtual void unregisterBleCommandListener(AbstractBleCommandListener& commandListener) = 0;

    virtual void setHidReportMap(etl::array_view<const uint16_t> reportLengths,
                                 etl::array_view<const uint8_t> reportMap) = 0;
    virtual void setDeviceName(etl::string_view name) = 0;
    virtual void setDeviceMacAddress(const MacAddress& mac) = 0;
    virtual void setDeviceProperties(const BLEDefs::bleDeviceProperties& deviceProperties) = 0;

    virtual void init() = 0;
    virtual void configure() = 0;
    virtual void enterDfu() = 0;
    virtual void forceDfu() = 0;
    virtual void enableBle() = 0;
    virtual void disableBle() = 0;
    virtual BLEDefs::ErrCode sendCommand(BleReadyForBleTxCommandRef cmd) = 0;
    virtual BLEDefs::ErrCode sendCommand(BleApplicationTxCommandRef cmd) = 0;
    virtual BLEDefs::ErrCode sendCommand(BleDfuModeTxCommandRef cmd) = 0;
    virtual BLEDefs::ErrCode sendRequest(BleApplicationTxRequestRef cmd) = 0;
};