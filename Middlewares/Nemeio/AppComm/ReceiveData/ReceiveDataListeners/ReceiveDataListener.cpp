/*
 * ReceiveDataListener.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: Labo
 */

#include <ReceiveData/ReceiveDataListeners/ReceiveDataListener.hpp>

ReceiveDataListener::ReceiveDataListener(AppCommDefs::ReceiveDataID dataId)
    : StreamDataListener(dataId)
{
}

uint8_t ReceiveDataListener::prepareReceive(const uint8_t* pHeader,
                                            size_t headerLen,
                                            std::size_t& retDataLen)
{
    return doPrepareReceive(pHeader, headerLen, retDataLen);
}

uint8_t ReceiveDataListener::receiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength)
{
    return doReceiveData(pData, len, retReceiveLength);
}

uint8_t ReceiveDataListener::terminateReceive()
{
    return doTerminateReceive();
}
