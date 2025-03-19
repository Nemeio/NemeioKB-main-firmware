/*
 * SendDataCmd.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#include "assertTools.h"
#include <SendData/SendDataCmd.hpp>
#include "AppComm.hpp"
#include "AppCommUtils.hpp"

SendDataCmd::SendDataCmd(IAppCommTxListener& appCommTxListener)
    : StreamDataCmd(appCommTxListener)
{
}

uint8_t SendDataCmd::doPrepare(SendDataListener& listener,
                               const uint8_t* pBuff,
                               std::size_t len,
                               std::size_t& retStreamDataLength,
                               WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL;
    uint32_t sendDataLength = 0;

    ASSERT(pBuff != nullptr);

    if(sizeof(sendDataLength) <= len)
    {
        std::size_t idx = 0;
        AppCommUtils::parseU32(pBuff, len, idx, sendDataLength, idx);
        pBuff += sizeof(sendDataLength);
        len -= sizeof(sendDataLength);
        ret = listener.prepareSend(pBuff, len, sendDataLength);
        if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
        {
            setSuccessResp(respBuff);
        }
    }

    retStreamDataLength = sendDataLength;

    return ret;
}

uint8_t SendDataCmd::doStreamData(SendDataListener& listener,
                                  std::size_t remainingStreamLength,
                                  const uint8_t* pBuff,
                                  std::size_t len,
                                  std::size_t& retFrameDataLength,
                                  WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL;

    ASSERT(pBuff != nullptr);

    if(remainingStreamLength >= len)
    {
        ret = listener.sendData(pBuff, len);
        if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
        {
            setSuccessResp(respBuff);
        }
    }

    retFrameDataLength = len;

    return ret;
}

uint8_t SendDataCmd::doTerminate(SendDataListener& listener,
                                 const uint8_t* pBuff,
                                 std::size_t len,
                                 WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    uint8_t ret = listener.terminateSend();

    if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
    {
        ret = listener.applySend();
        if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
        {
            setSuccessResp(respBuff);
        }
    }

    return ret;
}

void SendDataCmd::setSuccessResp(WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    const uint8_t ret = AppCommDefs::APPCOMMERR_SUCCESS;
    respBuff.cleanWrite(ret);
}
