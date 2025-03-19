#pragma once

#include "BleCmdPacket.hpp"
#include "AbstractBleCommandListener.hpp"
#include "SystemTypes.hpp"

class BleTxCmd
{
public:
    explicit BleTxCmd(BLESPICommands::CommandId id)
        : m_id(id)
    {
    }
    virtual ~BleTxCmd() = default;

    virtual void fill(BleCmdPacket& packet) const = 0;

    BLESPICommands::CommandId getId() const { return m_id; }

private:
    BLESPICommands::CommandId m_id;
};

class BleTxRxCmd : public BleTxCmd, public AbstractBleCommandListener
{
public:
    explicit BleTxRxCmd(BLESPICommands::CommandId id)
        : BleTxCmd(id)
        , AbstractBleCommandListener(id)
    {
    }
    virtual ~BleTxRxCmd() = default;
};

class BleTxRequest : public BleTxRxCmd
{
public:
    using BleTxRxCmd::BleTxRxCmd;
    virtual ~BleTxRequest() = default;

    virtual void onTimeout() = 0;
    void setTimeout(DelayMs timeout) { m_timeout = timeout; }
    DelayMs getTimeout() const { return m_timeout; }

private:
    static constexpr uint32_t DEFAULT_TIMEOUT_MS = 3000;
    DelayMs m_timeout{DelayMs(DEFAULT_TIMEOUT_MS)};
};