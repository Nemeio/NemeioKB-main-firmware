#pragma once

#include "BleTxCmd.hpp"

class BleSetAdvertisingCmd : public BleTxCmd
{
public:
    BleSetAdvertisingCmd();
    virtual ~BleSetAdvertisingCmd() = default;

    void setEnable(bool enable) { m_enable = enable; }
    void fill(BleCmdPacket& packet) const final;

private:
    bool m_enable{false};
};