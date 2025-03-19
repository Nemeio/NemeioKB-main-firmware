#pragma once

#include "BleTxCmd.hpp"

class BleDfuStartCmd : public BleTxCmd
{
public:
    BleDfuStartCmd();
    virtual ~BleDfuStartCmd() = default;

    void fill(BleCmdPacket& packet) const final;
};