/*
 * BleSerial.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: Labo
 */

#include "BleSerial.hpp"

BleSerial::BleSerial(IDataTransportRxListener& rxListener, IBleChip& bleChip)
    : AbstractDataTransport(rxListener)
    , m_bleChip(bleChip)
{
    m_bleSerialDataCmd.add_observer(*this);
    m_bleChip.registerBleCommandListener(m_bleSerialDataCmd);
}

BleSerial::~BleSerial()
{
    m_bleChip.unregisterBleCommandListener(m_bleSerialDataCmd);
}

void BleSerial::sendData(const uint8_t* pData, std::size_t len)
{
    while(len != 0)
    {
        size_t bytesToBeSent = m_bleSerialDataCmd.setData({pData, len});
        len -= bytesToBeSent;
        pData += bytesToBeSent;
        m_bleChip.sendCommand(
            BleReadyForBleTxCommandRef(etl::reference_wrapper(m_bleSerialDataCmd)));
    }
}

void BleSerial::notification(const BleSerialDataEvent& event)
{
    notifyReceivedData(event.data.data(), event.data.size());
}
