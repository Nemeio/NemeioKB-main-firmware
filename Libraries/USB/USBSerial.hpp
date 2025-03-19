/*
 * USBSerial.hpp
 *
 *  Created on: Jan 31, 2019
 *      Author: Labo
 */

#ifndef USB_USBSERIAL_HPP_
#define USB_USBSERIAL_HPP_

#include "AbstractDataTransport.hpp"
#include "IDataTransportRxListener.hpp"
#include "usbd_def.h"

class USBSerial : public AbstractDataTransport
{
public:
    USBSerial(IDataTransportRxListener& rxListener);
    virtual ~USBSerial() = default;

    void init();
    void sendData(const uint8_t* pData, std::size_t len);

private:
    USBD_HandleTypeDef* mpDev;

    static uint8_t receivedDataCallback(void* pListener, const uint8_t* pData, std::size_t len);
};

#endif /* USB_USBSERIAL_HPP_ */
