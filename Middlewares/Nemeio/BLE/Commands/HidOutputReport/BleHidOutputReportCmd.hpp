#pragma once

#include "AbstractBleCommandListener.hpp"
#include "BleHidOutputReportCmdObserver.hpp"
#include "etl/array.h"

class BleHidOutputReportCmd : public AbstractBleCommandListener,
                              public ProtectedObservable<BleHidOutputReportCmdObserver, 1>
{
public:
    BleHidOutputReportCmd();
    virtual ~BleHidOutputReportCmd() = default;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;
};