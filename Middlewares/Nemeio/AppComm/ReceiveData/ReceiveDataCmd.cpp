/*
 * ReceiveDataCmd.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#include <ReceiveData/ReceiveDataCmd.hpp>
#include <algorithm>

ReceiveDataCmd::ReceiveDataCmd(IAppCommTxListener& appCommTxListener)
    : ReceiveDataCmdBase::StreamDataCmd(appCommTxListener)
{
}

uint8_t ReceiveDataCmd::doPrepare(ReceiveDataListener& listener,
                                  const uint8_t* pBuff,
                                  std::size_t len,
                                  std::size_t& retStreamDataLength,
                                  WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    std::size_t receiveDataLength = 0;

    uint8_t ret = listener.prepareReceive(pBuff, len, receiveDataLength);

    StartResponse startResponse;
    startResponse.retCode = ret;

    size_t unusedIdx;
    AppCommUtils::writeU32(receiveDataLength,
                           reinterpret_cast<uint8_t*>(&startResponse.dataSize),
                           sizeof(startResponse.dataSize),
                           0,
                           unusedIdx);

    if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
    {
        setSuccessResp(respBuff, startResponse);
    }

    retStreamDataLength = receiveDataLength;

    return ret;
}

uint8_t ReceiveDataCmd::doStreamData(ReceiveDataListener& listener,
                                     std::size_t remainingStreamLength,
                                     const uint8_t* pBuff,
                                     std::size_t len,
                                     std::size_t& retFrameDataLength,
                                     WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    uint8_t ret = AppCommDefs::APPCOMMERR_STREAMDATA_PROTOCOL;
    uint32_t receiverMaxDataLength = 0;
    ReceiveResponse recResponse = {0};

    if(sizeof(receiverMaxDataLength) == len)
    {
        std::size_t idx = 0;
        AppCommUtils::parseU32(pBuff, len, idx, receiverMaxDataLength, idx);
        std::size_t maxDataLength = std::min(remainingStreamLength,
                                             std::min(static_cast<std::size_t>(
                                                          receiverMaxDataLength),
                                                      RECEIVE_FRAME_MAX_SIZE));

        ret = listener.receiveData(recResponse.data, maxDataLength, retFrameDataLength);
    }

    recResponse.retCode = ret;
    if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
    {
        setReceiveSuccessResp(respBuff, recResponse, retFrameDataLength);
    }

    return ret;
}

uint8_t ReceiveDataCmd::doTerminate(ReceiveDataListener& listener,
                                    const uint8_t* pBuff,
                                    std::size_t len,
                                    WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff)
{
    uint8_t ret = listener.terminateReceive();

    if(AppCommDefs::APPCOMMERR_SUCCESS == ret)
    {
        setSuccessResp(respBuff, ret);
    }

    return ret;
}

template<typename T>
void ReceiveDataCmd::setSuccessResp(WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff, T& resp)
{
    respBuff.cleanWrite(resp);
}

void ReceiveDataCmd::setReceiveSuccessResp(WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff,
                                           ReceiveResponse& resp,
                                           std::size_t frameDataLen)
{
    ASSERT(respBuff.size() >= sizeof(resp));
    const uint8_t* pResp = reinterpret_cast<const uint8_t*>(&resp);

    respBuff.cleanWrite(pResp, pResp + sizeof(resp.retCode) + frameDataLen);
}
