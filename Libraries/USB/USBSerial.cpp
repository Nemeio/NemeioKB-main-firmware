/*
 * USBSerial.cpp
 *
 *  Created on: Jan 31, 2019
 *      Author: Labo
 */

#include <USBSerial.hpp>
#include "usbd_custom_composite.h"

extern "C" USBD_HandleTypeDef hUsbDeviceFS;

void USBSerial::init()
{
    CDC_InitInterface(mpDev, this, receivedDataCallback);
}

USBSerial::USBSerial(IDataTransportRxListener& rxListener)
    : AbstractDataTransport(rxListener)
    , mpDev(&hUsbDeviceFS)
{
}

void USBSerial::sendData(const uint8_t* pData, std::size_t len)
{
    CDC_SendData(pData, len);
}

uint8_t USBSerial::receivedDataCallback(void* pListener, const uint8_t* pData, std::size_t len)
{
    if(pListener != NULL)
    {
        ((USBSerial*) pListener)->notifyReceivedData(pData, len);
    }

    return 0;
}
