#pragma once

#include "BleEnteredDfuCmdObserver.hpp"
#include "AbstractBleCommandListener.hpp"
#include "etl/array.h"

class BleEnteredDfuCmd : public AbstractBleCommandListener,
                         public ProtectedObservable<BleEnteredDfuCmdObserver, 1>
{
public:
    BleEnteredDfuCmd();
    virtual ~BleEnteredDfuCmd() = default;

    void receivedData(const uint8_t* pData, std::size_t dataLen) override;
};