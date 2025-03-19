#pragma once

#include "BleStartedCmdObserver.hpp"
#include "AbstractBleCommandListener.hpp"
#include "etl/array.h"

class BleNrfStartedCmd : public AbstractBleCommandListener,
                         public ProtectedObservable<BleStartedCmdObserver, 1>
{
public:
    BleNrfStartedCmd();
    virtual ~BleNrfStartedCmd() = default;

    void receivedData(const uint8_t* pData, std::size_t dataLen) override;
};