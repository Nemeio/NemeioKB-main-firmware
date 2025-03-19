/*
 * InterfaceHAL.cpp
 *
 *  Created on: 5 avr. 2017
 *      Author: cgarnier
 */

#include "UsbHID.h"
#include "usbd_conf.h"
#include "usbd_custom_composite.h"
#include "usbd_hid_if.h"
#include "assertTools.h"
#include "FreeRTOSUtils.hpp"

UsbHID::UsbHID()
    : AbstractHID()
    , mpdev(&hUsbDeviceFS)
    , mUsbHIDState(USB_HID_ACTIVE)
{
    BaseType_t ret = xTaskCreate(vUsbHIDTask,
                                 "UsbHIDTask",
                                 USB_HID_TASK_STACK_SIZE,
                                 this,
                                 USB_HID_TASK_PRIORITY,
                                 &mUsbHIDTaskHandle);
    ASSERT(ret == pdPASS);

    mOutputReportQueue = xQueueCreate(OUTPUT_REPORT_QUEUE_SIZE, OUTPUT_REPORT_SIZE);
    ASSERT(mOutputReportQueue != nullptr);

    HID_InitInterface(this, receivedOutputReportCallback);
}

void UsbHID::sendReport(const uint8_t* reportBuf, size_t reportBufSize)
{
    if(mUsbHIDState == USB_HID_ACTIVE)
    {
        USBD_CUSTOM_ACM_HID_HID_SendReport(mpdev, reportBuf, reportBufSize);
    }
}

void UsbHID::active()
{
    mUsbHIDState = USB_HID_ACTIVE;

    resumeUSB();
}

void UsbHID::inactive()
{
    mUsbHIDState = USB_HID_ACTIVE;
}

void UsbHID::longInactive()
{
    mUsbHIDState = USB_HID_ACTIVE;
}

void UsbHID::sleep()
{
}

void UsbHID::powerOff()
{
    mUsbHIDState = USB_HID_ACTIVE;
}

void UsbHID::suspend()
{
    mUsbHIDState = USB_HID_SUSPENDED;
}

void UsbHID::receivedOutputReportCallback(void* pListener, const uint8_t* pData, std::size_t len)
{
    if(OUTPUT_REPORT_SIZE == len && pListener != nullptr)
    {
        BaseType_t taskWoken = pdFALSE;
        xQueueSendFromISR(reinterpret_cast<UsbHID*>(pListener)->mOutputReportQueue,
                          pData,
                          &taskWoken);
        portEND_SWITCHING_ISR(taskWoken);
    }
}

void UsbHID::vUsbHIDTask(void* pvParameters)
{
    UsbHID* usbHID = reinterpret_cast<UsbHID*>(pvParameters);
    ASSERT(usbHID != nullptr);

    uint8_t data[OUTPUT_REPORT_SIZE];
    for(;;)
    {
        if(pdPASS
           == xQueueReceive(usbHID->mOutputReportQueue,
                            &data,
                            delayToSystemTicks(DelayMs::INFINITE_DELAY)))
        {
            usbHID->receivedOutputReport(data, OUTPUT_REPORT_SIZE);
        }
    }
}
