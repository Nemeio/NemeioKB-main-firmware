#pragma once

#include "BleEventCmdObserver.hpp"
#include "AbstractBleCommandListener.hpp"
#include "etl/array.h"

class BleEventCmd : public AbstractBleCommandListener,
                    public ProtectedObservable<BleEventCmdObserver, 1>
{
public:
    BleEventCmd();
    virtual ~BleEventCmd() = default;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;
};