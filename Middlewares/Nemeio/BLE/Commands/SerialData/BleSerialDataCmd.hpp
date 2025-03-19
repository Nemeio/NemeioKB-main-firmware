#pragma once

#include "BleTxCmd.hpp"
#include "BleSerialDataCmdObserver.hpp"

class BleSerialDataCmd : public BleTxRxCmd, public ProtectedObservable<BleSerialDataCmdObserver, 1>
{
public:
    BleSerialDataCmd();
    virtual ~BleSerialDataCmd() = default;

    size_t setData(etl::array_view<const uint8_t> data);

    void fill(BleCmdPacket& packet) const final;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;

private:
    static constexpr size_t MAX_SERIAL_DATA_TX_LEN = 128;

    etl::array_view<const uint8_t> m_data;
};