#pragma once

#include "ReceiveLayoutListCmd.hpp"
#include "CborUuidListCodec.hpp"
#include "LayoutCmdContext.hpp"

class ReceiveLayoutListCmdAssembly
{
public:
    explicit ReceiveLayoutListCmdAssembly(LayoutCmdContext& context)
        : m_cmd(context, m_codec)
    {
    }
    virtual ~ReceiveLayoutListCmdAssembly() = default;

    ReceiveLayoutListCmd& getItem() { return m_cmd; }

private:
    CborUuidListCodec m_codec;
    ReceiveLayoutListCmd m_cmd;
};