#pragma once

#include "BleTxCmd.hpp"
#include "BLEDefs.hpp"

class BleSetRadioModeCmd : public BleTxCmd
{
public:
    BleSetRadioModeCmd();
    virtual ~BleSetRadioModeCmd() = default;

    void setMode(BLEDefs::TestMode mode) { m_mode = mode; }
    void fill(BleCmdPacket& packet) const final;

private:
    BLEDefs::TestMode m_mode{BLEDefs::TestMode::UNKNOWN};
};