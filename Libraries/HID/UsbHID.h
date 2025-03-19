/*
 * AbstractHID.h
 *
 *  Created on: 5 avr. 2017
 *      Author: cgarnier
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBHID_H_
#define __USBHID_H_


/* Includes ------------------------------------------------------------------*/
#include "AbstractHID.h"
#include "usbd_def.h"
#include "IPowerManageable.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

extern "C" USBD_HandleTypeDef hUsbDeviceFS;

class UsbHID : public AbstractHID, public IPowerManageable
{
public:
    //Methods
    UsbHID();
    virtual ~UsbHID() = default;

    void active();
    void inactive();
    void longInactive();
    void sleep();
    void powerOff();
    void suspend();

private:
    static constexpr UBaseType_t USB_HID_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE USB_HID_TASK_STACK_SIZE = 256;
    static constexpr std::size_t OUTPUT_REPORT_QUEUE_SIZE = 10;
    static constexpr std::size_t OUTPUT_REPORT_SIZE = 2;

    typedef enum USB_HID_STATE_
    {
        USB_HID_ACTIVE,
        USB_HID_SUSPENDED
    } USB_HID_STATE;

    USBD_HandleTypeDef* mpdev;
    USB_HID_STATE mUsbHIDState;

    TaskHandle_t mUsbHIDTaskHandle;
    QueueHandle_t mOutputReportQueue;

    void sendReport(const uint8_t* reportBuf, size_t reportBufSize);

    static void receivedOutputReportCallback(void* pListener, const uint8_t* pData, std::size_t len);

    static void vUsbHIDTask(void* pvParameters);
};
#endif /* ABSTRACTHID_H_ */
