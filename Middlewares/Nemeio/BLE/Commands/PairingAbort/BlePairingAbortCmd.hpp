#pragma once

#include "AbstractBleCommandListener.hpp"
#include "BlePairingAbortCmdObserver.hpp"
#include "etl/array.h"

class BlePairingAbortCmd : public AbstractBleCommandListener,
                           public ProtectedObservable<BlePairingAbortCmdObserver, 1>
{
public:
    BlePairingAbortCmd();
    virtual ~BlePairingAbortCmd() = default;

    void receivedData(const uint8_t* pData, std::size_t dataLen) override;
};