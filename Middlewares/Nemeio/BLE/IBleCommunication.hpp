#pragma once

#include "BLEDefs.hpp"
#include "AbstractBleCommandListener.hpp"
#include "MacAddress.hpp"
#include "BleTxCmd.hpp"
#include "IPowerManageable.h"

class IBleCommunication
{
public:
    IBleCommunication() = default;
    virtual ~IBleCommunication() = default;

    virtual void registerCommandListener(AbstractBleCommandListener& commandListener) = 0;
    virtual void unregisterListener(AbstractBleCommandListener& listener) = 0;

    virtual BLEDefs::ErrCode init() = 0;
    virtual BLEDefs::ErrCode powerOff() = 0;
    virtual BLEDefs::ErrCode sendCommand(BleTxCmd& cmd) = 0;
    virtual BLEDefs::ErrCode sendRequest(BleTxRequest& cmd) = 0;
    virtual BLEDefs::ErrCode forceDfu() = 0;
};
