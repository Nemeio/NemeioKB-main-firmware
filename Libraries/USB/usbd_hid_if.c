/*
 * usbd_hid_if.c
 *
 *  Created on: Sep 17, 2019
 *      Author: Labo
 */

#include "usbd_hid_if.h"

#define HID_SET_REPORT_TYPE_OUTPUT 0x02
#define HID_SET_REPORT_INTERFACE 0x0002

typedef struct _HIDListener_t
{
    void* pListener;
    void (*outputReportCallback)(void* pListener, const uint8_t* pData, size_t len);
} HIDListener_t;

static HIDListener_t HIDListener = {NULL, NULL};

static void HID_SetReport(
    uint8_t type, uint8_t id, uint16_t interface, const uint8_t* data, uint16_t len);
static bool HID_GetOutputReportId(const uint8_t* data, uint16_t len, uint8_t* pReportId);

USBD_HID_ItfTypeDef USBD_HID_Interface = {HID_SetReport};

static void HID_SetReport(
    uint8_t type, uint8_t id, uint16_t interface, const uint8_t* data, uint16_t len)
{
    if(len > sizeof(uint8_t) && HID_SET_REPORT_TYPE_OUTPUT == type
       && HID_SET_REPORT_INTERFACE == interface)
    {
        uint8_t reportId = 0;

        if(HID_GetOutputReportId(data, len, &reportId) && reportId == id)
        {
            if(HIDListener.pListener != NULL && HIDListener.outputReportCallback != NULL)
            {
                HIDListener.outputReportCallback(HIDListener.pListener, data, len);
            }
        }
    }
}

bool HID_GetOutputReportId(const uint8_t* data, uint16_t len, uint8_t* pReportId)
{
    bool bRet = false;

    if(pReportId != NULL && len > sizeof(uint8_t))
    {
        *pReportId = data[0];
        bRet = true;
    }

    return bRet;
}


void HID_InitInterface(void* pListener, void (*outputReportCallback)(void*, const uint8_t*, size_t))
{
    HIDListener.pListener = pListener;
    HIDListener.outputReportCallback = outputReportCallback;
}
