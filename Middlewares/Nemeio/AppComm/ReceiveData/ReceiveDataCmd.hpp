/*
 * ReceiveDataCmd.hpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_RECEIVEDATA_RECEIVEDATACMD_HPP_
#define APPCOMM_RECEIVEDATA_RECEIVEDATACMD_HPP_

#include "../StreamDataCmd/StreamDataCmd.hpp"
#include "ReceiveDataListeners/ReceiveDataListener.hpp"

typedef StreamDataCmd<ReceiveDataListener, AppCommDefs::AppCommCmd::RCVDATA, AppCommDefs::ReceiveDataID>
    ReceiveDataCmdBase;

class ReceiveDataCmd : public ReceiveDataCmdBase
{
public:
    ReceiveDataCmd(IAppCommTxListener& appCommTxListener);
    virtual ~ReceiveDataCmd() = default;

private:
    struct __attribute__((__packed__)) StartResponse
    {
        uint8_t retCode;
        uint32_t dataSize;
    };

    static constexpr std::size_t RECEIVE_FRAME_MAX_SIZE = 128;
    struct __attribute__((__packed__)) ReceiveResponse
    {
        uint8_t retCode;
        uint8_t data[RECEIVE_FRAME_MAX_SIZE];
    };

    uint8_t doPrepare(ReceiveDataListener& listener,
                      const uint8_t* pBuff,
                      std::size_t len,
                      std::size_t& retStreamDataLength,
                      WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
    uint8_t doStreamData(ReceiveDataListener& listener,
                         std::size_t remainingStreamLength,
                         const uint8_t* pBuff,
                         size_t len,
                         std::size_t& retFrameDataLength,
                         WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
    uint8_t doTerminate(ReceiveDataListener& listener,
                        const uint8_t* pBuff,
                        std::size_t len,
                        WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
    template<typename T>
    void setSuccessResp(WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff, T& resp);
    void setReceiveSuccessResp(WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff,
                               ReceiveResponse& resp,
                               std::size_t frameDataLen);
};

#endif /* APPCOMM_RECEIVEDATA_RECEIVEDATACMD_HPP_ */
