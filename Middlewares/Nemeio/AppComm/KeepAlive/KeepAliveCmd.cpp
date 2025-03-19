/*
 * KeepAliveCmd.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: Labo
 */

#include "KeepAliveCmd.hpp"
#include <stddef.h>
#include "assertTools.h"

KeepAliveCmd::KeepAliveCmd(IAppCommTxListener& appCommTxListener,
                           IKeepAliveManager& keepAliveMgr,
                           IKeyboardStatus& keyboardStatus,
                           IKeepAliveRespCodec& codec)
    : AppCommCmd(AppCommDefs::AppCommCmd::KEEPALIVE, appCommTxListener)
    , m_keepAliveMgr(keepAliveMgr)
    , m_keyboardStatus(keyboardStatus)
    , m_codec(codec)
{
}

void KeepAliveCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    if(dataLen != 0)
    {
        return;
    }

    doKeepAlive();
}

void KeepAliveCmd::doKeepAlive()
{
    m_keepAliveMgr.receivedKeepAlive();

    m_keyboardStatus.refresh();

    etl::array<std::byte, MAX_KEEP_ALIVE_RESP_SIZE> responseBuffer;
    ssize_t responseSize = m_codec.encode(m_keyboardStatus, responseBuffer);

    ASSERT(responseSize >= 0);

    transmitResponse(reinterpret_cast<const uint8_t*>(responseBuffer.data()), responseSize);
}
