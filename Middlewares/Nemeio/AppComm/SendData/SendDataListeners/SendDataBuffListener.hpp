/*
 * SendDataBuffListener.hpp
 *
 *  Created on: Feb 12, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_SENDDATA_SENDDATABUFFLISTENER_HPP_
#define APPCOMM_SENDDATA_SENDDATABUFFLISTENER_HPP_

#include "SendDataListener.hpp"
#include <memory>

class SendDataBuffListener : public SendDataListener
{
public:
    SendDataBuffListener(AppCommDefs::SendDataID dataId);
    virtual ~SendDataBuffListener() = default;

    SendDataBuffListener(SendDataBuffListener&) = delete;
    SendDataBuffListener& operator=(const SendDataBuffListener&) = delete;
    SendDataBuffListener(SendDataBuffListener&&) = delete;
    SendDataBuffListener const& operator=(SendDataBuffListener&&) = delete;

protected:
    std::unique_ptr<uint8_t[]> mpBuffer = nullptr;

    uint8_t doPrepareSend(const uint8_t* pHeader, size_t headerLen, size_t dataLen) override;

private:
    uint32_t mSizeCompleted = 0;

    uint8_t doSendData(const uint8_t* pData, size_t len);
    uint8_t doTerminateSend();
    virtual uint8_t doApplySend() = 0;
    void doReset();
};

#endif /* APPCOMM_SENDDATA_SENDDATABUFFLISTENER_HPP_ */
