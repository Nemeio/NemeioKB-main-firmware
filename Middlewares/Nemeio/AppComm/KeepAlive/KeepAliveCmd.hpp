#pragma once

#include "AppCommCmd.hpp"
#include "IKeepAliveManager.hpp"
#include "IKeyboardStatus.hpp"
#include "IKeepAliveRespCodec.hpp"

class KeepAliveCmd : public AppCommCmd
{
public:
    KeepAliveCmd(IAppCommTxListener& appCommTxListener,
                 IKeepAliveManager& keepAliveMgr,
                 IKeyboardStatus& keyboardStatus,
                 IKeepAliveRespCodec& codec);
    virtual ~KeepAliveCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    static constexpr size_t MAX_KEEP_ALIVE_RESP_SIZE = 9;
    IKeepAliveManager& m_keepAliveMgr;
    IKeyboardStatus& m_keyboardStatus;
    IKeepAliveRespCodec& m_codec;

    void doKeepAlive();
};
