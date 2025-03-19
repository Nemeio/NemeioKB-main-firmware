#ifndef MOCKS_MOCKBLECHIP_HPP_
#define MOCKS_MOCKBLECHIP_HPP_

#include "gmock/gmock.h"
#include "IBleChip.hpp"

class MockBleChip : public IBleChip
{
public:
    MOCK_METHOD(void,
                registerBleCommandListener,
                (AbstractBleCommandListener & commandListener),
                (override));
    MOCK_METHOD(void,
                unregisterBleCommandListener,
                (AbstractBleCommandListener & commandListener),
                (override));

    MOCK_METHOD(void,
                setHidReportMap,
                (etl::array_view<const uint16_t> reportLengths,
                 etl::array_view<const uint8_t> reportMap),
                (override));
    MOCK_METHOD(void, setDeviceName, (etl::string_view name), (override));
    MOCK_METHOD(void, setDeviceMacAddress, (const MacAddress& mac), (override));
    MOCK_METHOD(void,
                setDeviceProperties,
                (const BLEDefs::bleDeviceProperties& deviceProperties),
                (override));

    MOCK_METHOD(void, init, (), (override));
    MOCK_METHOD(void, configure, (), (override));
    MOCK_METHOD(void, enterDfu, (), (override));
    MOCK_METHOD(void, forceDfu, (), (override));
    MOCK_METHOD(void, enableBle, (), (override));
    MOCK_METHOD(void, disableBle, (), (override));
    MOCK_METHOD(BLEDefs::ErrCode, sendCommand, (BleReadyForBleTxCommandRef cmd), (override));
    MOCK_METHOD(BLEDefs::ErrCode, sendCommand, (BleApplicationTxCommandRef cmd), (override));
    MOCK_METHOD(BLEDefs::ErrCode, sendCommand, (BleDfuModeTxCommandRef cmd), (override));
    MOCK_METHOD(BLEDefs::ErrCode, sendRequest, (BleApplicationTxRequestRef cmd), (override));
};

#endif /* MOCKS_MOCKBLECHIP_HPP_ */
