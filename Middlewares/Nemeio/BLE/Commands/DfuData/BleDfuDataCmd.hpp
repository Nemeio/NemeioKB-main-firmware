#pragma once

#include "BleTxCmd.hpp"
#include "BleDfuDataCmdObserver.hpp"
#include "etl/array.h"

class BleDfuDataCmd : public BleTxRxCmd, public ProtectedObservable<BleDfuDataCmdObserver, 1>
{
public:
    BleDfuDataCmd();
    virtual ~BleDfuDataCmd() = default;

    void setData(const uint8_t* data, size_t len);
    void fill(BleCmdPacket& packet) const final;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;

private:
    const uint8_t* m_data{nullptr};
    size_t m_len{0};
};