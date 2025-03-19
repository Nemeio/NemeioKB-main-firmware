#pragma once

#include "BleTxCmd.hpp"
#include "BleFwVersionObserver.hpp"
#include "Version.h"
#include "etl/array.h"

class BleGetFwVersionCmd : public BleTxRequest, public ProtectedObservable<BleFwVersionObserver, 1>
{
public:
    BleGetFwVersionCmd();
    virtual ~BleGetFwVersionCmd() = default;

    void fill(BleCmdPacket& packet) const final;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;
    void onTimeout() final;
};