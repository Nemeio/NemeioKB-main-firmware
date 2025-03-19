#pragma once

#include "BleTxCmd.hpp"
#include "BLEDefs.hpp"

class BleDfuCommitVersionCmd : public BleTxCmd
{
public:
    BleDfuCommitVersionCmd();
    virtual ~BleDfuCommitVersionCmd() = default;

    void setVersion(uint16_t major, uint8_t minor)
    {
        m_version.major = major;
        m_version.minor = minor;
    }
    void fill(BleCmdPacket& packet) const final;

private:
    BLEDefs::FirmwareVersion m_version{0, 0};
};