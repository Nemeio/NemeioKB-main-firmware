/*
 * SendDataListener.hpp
 *
 *  Created on: Feb 5, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SENDDATA_SENDDATALISTENER_HPP_
#define APPCOMM_SENDDATA_SENDDATALISTENER_HPP_

#include "AppCommDefs.hpp"
#include <cstddef>
#include "../StreamDataCmd/StreamDataListener.hpp"

class SendDataListener : public StreamDataListener<AppCommDefs::SendDataID>
{
public:
    SendDataListener(AppCommDefs::SendDataID dataId);
    virtual ~SendDataListener() = default;

    uint8_t prepareSend(const uint8_t* pHeader, size_t headerLen, uint32_t dataLen);
    uint8_t sendData(const uint8_t* pData, size_t len);
    uint8_t terminateSend();
    uint8_t applySend();

protected:
    uint32_t mDataLength;

private:
    virtual uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen) = 0;
    virtual uint8_t doSendData(const uint8_t* pData, size_t len) = 0;
    virtual uint8_t doTerminateSend() = 0;
    virtual uint8_t doApplySend() = 0;
};

#endif /* APPCOMM_SENDDATA_SENDDATALISTENER_HPP_ */
