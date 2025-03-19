#pragma once

#include "BleTxCmd.hpp"
#include "MacAddress.hpp"

class BleSetMacAddressCmd : public BleTxCmd
{
public:
    BleSetMacAddressCmd();
    virtual ~BleSetMacAddressCmd() = default;

    void setMacAddress(const MacAddress& mac) { m_mac = mac; }
    void fill(BleCmdPacket& packet) const final;

private:
    MacAddress m_mac{etl::array<uint8_t, MacAddress::SIZE>{0}};
};