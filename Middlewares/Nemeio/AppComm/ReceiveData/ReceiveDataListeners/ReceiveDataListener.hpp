/*
 * ReceiveDataListener.hpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_RECEIVEDATA_RECEIVEDATALISTENERS_RECEIVEDATALISTENER_HPP_
#define APPCOMM_RECEIVEDATA_RECEIVEDATALISTENERS_RECEIVEDATALISTENER_HPP_

#include "../StreamDataCmd/StreamDataListener.hpp"
#include <iterator>

class ReceiveDataListener : public StreamDataListener<AppCommDefs::ReceiveDataID>
{
public:
    ReceiveDataListener(AppCommDefs::ReceiveDataID dataId);
    virtual ~ReceiveDataListener() = default;

    uint8_t prepareReceive(const uint8_t* pHeader, size_t headerLen, std::size_t& retDataLen);
    uint8_t receiveData(uint8_t* pData, std::size_t len, std::size_t& retReceiveLength);
    uint8_t terminateReceive();

private:
    virtual uint8_t doPrepareReceive(const uint8_t* pHeader,
                                     size_t headerLen,
                                     std::size_t& retDataLen) = 0;
    virtual uint8_t doReceiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength) = 0;
    virtual uint8_t doTerminateReceive() = 0;
};

#endif /* APPCOMM_RECEIVEDATA_RECEIVEDATALISTENERS_RECEIVEDATALISTENER_HPP_ */
