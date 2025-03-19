#pragma once

#include "BleTxCmd.hpp"
#include "BleNumericComparisonCmdObserver.hpp"
#include "etl/array.h"

class BleNumericComparisonCmd : public BleTxRxCmd,
                                public ProtectedObservable<BleNumericComparisonCmdObserver, 1>
{
public:
    BleNumericComparisonCmd();
    virtual ~BleNumericComparisonCmd() = default;

    void setAccept(bool accept) { m_accept = accept; }
    void fill(BleCmdPacket& packet) const final;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;

private:
    bool m_accept{false};
};