/*
 * SendDataCmd.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SENDDATA_SENDDATACMD_HPP_
#define APPCOMM_SENDDATA_SENDDATACMD_HPP_

#include "../StreamDataCmd/StreamDataCmd.hpp"
#include "SendDataListener.hpp"
#include "AppCommDefs.hpp"

typedef StreamDataCmd<SendDataListener, AppCommDefs::AppCommCmd::SENDDATA, AppCommDefs::SendDataID>
    SendDataCmdBase;

class SendDataCmd : public SendDataCmdBase
{
public:
    SendDataCmd(IAppCommTxListener& appCommTxListener);
    virtual ~SendDataCmd() = default;

private:
    uint8_t doPrepare(SendDataListener& listener,
                      const uint8_t* pBuff,
                      std::size_t len,
                      std::size_t& retStreamDataLength,
                      WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
    uint8_t doStreamData(SendDataListener& listener,
                         std::size_t remainingStreamLength,
                         const uint8_t* pBuff,
                         size_t len,
                         std::size_t& retFrameDataLength,
                         WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
    uint8_t doTerminate(SendDataListener& listener,
                        const uint8_t* pBuff,
                        std::size_t len,
                        WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);

    void setSuccessResp(WriteBuffer<uint8_t, MAX_RESPONSE_SIZE>& respBuff);
};


#endif /* APPCOMM_SENDDATA_SENDDATACMD_HPP_ */
