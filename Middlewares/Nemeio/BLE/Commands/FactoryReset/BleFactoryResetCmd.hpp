#pragma once

#include "BleTxCmd.hpp"
#include "BleFactoryResetCmdObserver.hpp"
#include "Version.h"
#include "etl/array.h"

class BleFactoryResetCmd : public BleTxRequest,
                           public ProtectedObservable<BleFactoryResetCmdObserver, 1>
{
public:
    BleFactoryResetCmd();
    virtual ~BleFactoryResetCmd() = default;

    void fill(BleCmdPacket& packet) const final;

    void receivedData(const uint8_t* pData, std::size_t dataLen) final;
    void onTimeout() final;
};