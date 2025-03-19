/*
 * BleSerial.hpp
 *
 *  Created on: Jun 17, 2019
 *      Author: Labo
 */

#ifndef BLE_BLESERIAL_BLESERIAL_HPP_
#define BLE_BLESERIAL_BLESERIAL_HPP_

#include "AbstractDataTransport.hpp"
#include "IDataTransportRxListener.hpp"
#include "IBleChip.hpp"
#include "BleSerialDataCmd.hpp"
#include "cpp_helper.hpp"

class BleSerial : public AbstractDataTransport, public BleSerialDataCmdObserver
{
public:
    BleSerial(IDataTransportRxListener& rxListener, IBleChip& bleChip);
    virtual ~BleSerial();
    COPYABLE_MOVABLE(BleSerial, delete);

    void sendData(const uint8_t* pData, std::size_t len) final;
    void notification(const BleSerialDataEvent& event) final;

private:
    IBleChip& m_bleChip;
    BleSerialDataCmd m_bleSerialDataCmd;
};

#endif /* BLE_BLESERIAL_BLESERIAL_HPP_ */
