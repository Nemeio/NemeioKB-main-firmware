/*
 *
 *  Created on: 21 mars 2017
 *      Author: cgarnier
 */

#include "usbd_custom_composite.h"
#include "usbd_hid.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "usbd_ctlreq.h"
#include "usbd_conf.h"

#include "usbPowerState.h"


#include "usb_profile_cdc_acm.h"
#include "usb_profile_hid.h"
#include "usb_composite_api.h"
#include "usbd_hid_if.h"

#include "cmsis_os.h"
#include "assertTools.h"

static uint8_t USBD_Custom_Composite_Init(USBD_HandleTypeDef* pdev, uint8_t cfgidx);

static uint8_t USBD_Custom_Composite_DeInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx);

static uint8_t USBD_Custom_Composite_Setup(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req);

static uint8_t* USBD_Custom_Composite_GetCfgDesc(uint16_t* length);

static uint8_t* USBD_Custom_Composite_GetDeviceQualifierDesc(uint16_t* length);

static uint8_t USBD_Custom_Composite_DataIn(USBD_HandleTypeDef* pdev, uint8_t epnum);

static uint8_t USBD_Custom_Composite_DataOut(USBD_HandleTypeDef* pdev, uint8_t epnum);

static uint8_t USBD_Custom_Composite_EP0_RxReady(USBD_HandleTypeDef* pdev);

static uint8_t USBD_Custom_Composite_SoF(struct _USBD_HandleTypeDef* pdev);

static uint8_t USBD_Custom_Composite_ISO_IN_Incomplete(struct _USBD_HandleTypeDef* pdev,
                                                       uint8_t epnum);

static uint8_t USBD_Custom_Composite_ISO_OUT_Incomplete(struct _USBD_HandleTypeDef* pdev,
                                                        uint8_t epnum);

USBD_ClassTypeDef USBD_CUSTOM_ACM_HID = {
    USBD_Custom_Composite_Init,
    USBD_Custom_Composite_DeInit,
    USBD_Custom_Composite_Setup,
    NULL, /*EP0_TxSent*/
    USBD_Custom_Composite_EP0_RxReady,
    /*EP0_RxReady*/               /* STATUS STAGE IN */
    USBD_Custom_Composite_DataIn, /*DataIn*/
    USBD_Custom_Composite_DataOut,
    USBD_Custom_Composite_SoF, /*SOF */
    USBD_Custom_Composite_ISO_IN_Incomplete,
    USBD_Custom_Composite_ISO_OUT_Incomplete,
    USBD_Custom_Composite_GetCfgDesc,
    USBD_Custom_Composite_GetCfgDesc,
    USBD_Custom_Composite_GetCfgDesc,
    USBD_Custom_Composite_GetDeviceQualifierDesc,
};

#define PROFILE_SIZE \
    (USB_INTERFACE_ASSOCIATION_DESCRIPTOR_SIZE + USB_CDC_ACM_PROFILE_SIZE + USB_HID_PROFILE_SIZE)
#define PROFILE_INTERFACE_NUMBER \
    (USB_CDC_ACM_PROFILE_INTERFACE_NUMBER + USB_HID_PROFILE_INTERFACE_NUMBER)

#define CDC_ACM_FIRST_INTERFACE_INDEX (0)
#define CDC_ACM_LAST_INTERFACE_INDEX \
    (CDC_ACM_FIRST_INTERFACE_INDEX + USB_CDC_ACM_PROFILE_INTERFACE_NUMBER - 1)

#define HID_FIRST_INTERFACE_INDEX (CDC_ACM_LAST_INTERFACE_INDEX + 1)
#define HID_LAST_INTERFACE_INDEX (HID_FIRST_INTERFACE_INDEX + USB_HID_PROFILE_INTERFACE_NUMBER - 1)

__ALIGN_BEGIN static uint8_t USBD_Custom_Composite_CfgDesc[] __ALIGN_END = {

    USB_COMPOSITE_CONFIGURATION(PROFILE_SIZE, PROFILE_INTERFACE_NUMBER),

    USB_INTERFACE_ASSOCIATION_DESCRIPTOR(CDC_ACM_FIRST_INTERFACE_INDEX,
                                         USB_CDC_ACM_PROFILE_INTERFACE_NUMBER,
                                         CDC_ACM_INTERFACE_0_INTERFACE_CLASS,
                                         CDC_ACM_INTERFACE_0_INTERFACE_SUBCLASS,
                                         CDC_ACM_INTERFACE_0_INTERFACE_PROTOCOL,
                                         CDC_ACM_INTERFACE_0_INTERFACE),

    USB_CDC_ACM_PROFILE(CDC_ACM_FIRST_INTERFACE_INDEX),

    USB_HID_PROFILE(HID_FIRST_INTERFACE_INDEX)};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t
    USBD_Custom_Composite_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
        USB_LEN_DEV_QUALIFIER_DESC,
        USB_DESC_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02, //bcdUSB
        0xEF, //bDeviceClass
        0x02, //bDeviceSubClass
        0x01, //bDeviceProtocol
        0x40, //bMaxPacketSize0
        0x01, //bNumConfigurations
        0x00, //bReserved
};

/**
  * @brief  USBD_CUSTOM_HID_SendReport
  *         Send CUSTOM_HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */

uint8_t USBD_CUSTOM_HID_SendReport(USBD_HandleTypeDef* pdev, uint8_t* report, uint16_t len)
{
    return (USBD_HID_SendReport(pdev, report, len));
}

static uint8_t USBD_Custom_Composite_Init(USBD_HandleTypeDef* pdev, uint8_t cfgidx)
{
    uint8_t ret = 0; //in usbd_def.h USBD_OK = 0;
    /*Init ACM*/

    USBD_CDC_RegisterInterface(pdev, &USBD_CDC_Interface_fops_FS);
    USBD_CDC.Init(pdev, cfgidx);

    /*Init HID*/
    USBD_HID_RegisterInterface(pdev, &USBD_HID_Interface);
    USBD_HID.Init(pdev, cfgidx);

    return ret;
}

static uint8_t USBD_Custom_Composite_DeInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx)
{
    uint8_t ret = 0;
    /*DeInit ACM*/
    USBD_CDC.DeInit(pdev, cfgidx);

    /*DeInit HID*/
    USBD_HID.DeInit(pdev, cfgidx);
    return ret;
}

static uint8_t USBD_Custom_Composite_Setup(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req)
{
    uint8_t ret = 0;
    uint8_t interfaceNumber = LOBYTE(req->wIndex);

    if(interfaceNumber >= CDC_ACM_FIRST_INTERFACE_INDEX
       && interfaceNumber <= CDC_ACM_LAST_INTERFACE_INDEX)
    {
        ret = USBD_CDC.Setup(pdev, req);
    }
    else if(interfaceNumber >= HID_FIRST_INTERFACE_INDEX
            && interfaceNumber <= HID_LAST_INTERFACE_INDEX)
    {
        ret = (USBD_HID.Setup(pdev, req));
    }
    return ret;
}

/**
  * @brief  USBD_Custom_Composite_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t* USBD_Custom_Composite_GetCfgDesc(uint16_t* length)
{
    *length = sizeof(USBD_Custom_Composite_CfgDesc);
    enum USB_POWER_CONNECTION_STATE state = PLUG_IN_PC;
    pushUsbState(state);

    return USBD_Custom_Composite_CfgDesc;
}


/**
* @brief  USBD_Custom_Composite_GetDeviceQualifierDesc
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/

static uint8_t* USBD_Custom_Composite_GetDeviceQualifierDesc(uint16_t* length)
{
    *length = sizeof(USBD_Custom_Composite_DeviceQualifierDesc);
    return USBD_Custom_Composite_DeviceQualifierDesc;
}

/**
  * @brief  USBD_Custom_Composite_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */

static uint8_t USBD_Custom_Composite_DataIn(USBD_HandleTypeDef* pdev, uint8_t epnum)
{
    uint8_t ret = 0;
    epnum &= 0x0F;

    if(epnum == (CDC_CMD_EP & 0x0F) || epnum == (CDC_IN_EP & 0x0f))
    {
        ret = USBD_CDC.DataIn(pdev, epnum);
    }
    else if(epnum == (HID_EPIN_ADDR & 0x0F))
    {
        ret = (USBD_HID.DataIn(pdev, epnum));
    }
    return ret;
}

/**
  * @brief  USBD_Custom_Composite_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */

static uint8_t USBD_Custom_Composite_DataOut(USBD_HandleTypeDef* pdev, uint8_t epnum)
{
    uint8_t ret = 0;
    epnum &= 0x0F;
    if(epnum == (CDC_OUT_EP & 0x0F))
    {
        ret = USBD_CDC.DataOut(pdev, epnum);
    }

    return ret;
}

/**
  * @brief  USBD_CUSTOM_HID_EP0_RxReady
  *         Handles control request data.
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_Custom_Composite_EP0_RxReady(USBD_HandleTypeDef* pdev)
{
    uint8_t ret = 0;

    USBD_CDC.EP0_RxReady(pdev);
    USBD_HID.EP0_RxReady(pdev);

    return ret;
}

static uint8_t USBD_Custom_Composite_SoF(struct _USBD_HandleTypeDef* pdev)
{
    uint8_t ret = USBD_OK;
    return ret;
}

static uint8_t USBD_Custom_Composite_ISO_IN_Incomplete(struct _USBD_HandleTypeDef* pdev,
                                                       uint8_t epnum)
{
    uint8_t ret = 0;
    return ret;
}

static uint8_t USBD_Custom_Composite_ISO_OUT_Incomplete(struct _USBD_HandleTypeDef* pdev,
                                                        uint8_t epnum)
{
    uint8_t ret = 0;
    return ret;
}

uint8_t USBD_CUSTOM_ACM_HID_CDC_Send(USBD_HandleTypeDef* pdev, uint8_t* pData, size_t len)
{
    uint8_t ret = 0;

    if(len > UINT16_MAX)
    {
        return USBD_FAIL;
    }

    ret = CDC_Transmit_FS(pdev, pData, len);

    return ret;
}

uint8_t USBD_CUSTOM_ACM_HID_HID_SendReport(USBD_HandleTypeDef* pdev,
                                           const uint8_t* report,
                                           size_t len)
{
    uint8_t ret = 0;

    if(len > UINT16_MAX)
    {
        return USBD_FAIL;
    }

    ret = USBD_HID_SendReport(pdev, (uint8_t*) report, len);

    return ret;
}

void USBD_CUSTOM_ACM_HID_HID_Init()
{
    USBD_HID_Init_Sync();
}
