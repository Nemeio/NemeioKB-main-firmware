/**
  ******************************************************************************
  * @file    usbd_hid.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick
  *             - Collection : Application 
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "assertTools.h"

static SemaphoreHandle_t mUSBReadySem;
static USBD_HID_HandleTypeDef* USBD_HID_Handle = NULL;
static USBD_HID_ItfTypeDef* pUSBD_HID_Interface = NULL;
static USBD_HID_HandleTypeDef memHIDHandle;
/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID 
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_HID_Private_Defines
  * @{
  */
#define DELAY_SEND_REPORT_MS 1000
/**
  * @}
  */


/** @defgroup USBD_HID_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */
uint8_t USBD_HID_Init(USBD_HandleTypeDef* pdev, uint8_t cfgidx);

uint8_t USBD_HID_DeInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx);

uint8_t USBD_HID_Setup(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req);

uint8_t* USBD_HID_GetCfgDesc(uint16_t* length);

uint8_t* USBD_HID_GetDeviceQualifierDesc(uint16_t* length);

uint8_t USBD_HID_DataIn(USBD_HandleTypeDef* pdev, uint8_t epnum);

uint8_t USBD_HID_EP0_RxReady(void* pdev);
/**
  * @}
  */
__ALIGN_BEGIN uint8_t HID_KB_ReportDesc[HID_KB_REPORT_DESC_SIZE] __ALIGN_END = {
    0x05, 0x0C,       //Usage page consumer
    0x09, 0x01,       //Usage (Consumer control)
    0xA1, 0x01,       //collection (application)
    0x85, 0x01,       //Report_ID 1
    0x15, 0x00,       //Logical_minimum (0)
    0x25, 0x01,       //Logical maximum (1)
    0x75, 0x01,       //Report_Size(1)
    0x95, 0x06,       //Report_Count (6)
    0x09, 0xE9,       //        Usage (Volume Up)
    0x09, 0xEA,       //        Usage (Volume Down)
    0x09, 0xE2,       //        Usage (Volume Mute)
    0x0A, 0x21, 0x02, //AC "Search
    0x09, 0x6F,       //Display Brightness Increment (Req #HUTRR41)
    0x09, 0x70,       //Display Brightness Decrement (Req #HUTRR41)
    0x81, 0x06,       //Input
    0x75, 0x0A,       //Report_Size(1)
    0x95, 0x01,       //Report_Count(1)//Padding to reach a multiple of bytes
    0x81, 0x03,       //Input (Constant)
    0xC0,             //End collection
    0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,       // USAGE (Keyboard)
    0xa1, 0x01,       // COLLECTION (Application)
    0x85, 0x02,       //Report_ID 2
    0x75, 0x01,       //   REPORT_SIZE (1)
    0x95, 0x08,       //   REPORT_COUNT (8)
    0x05, 0x07,       //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0xe0,       //   USAGE_MINIMUM (Keyboard LeftControl)(224)
    0x29, 0xe7,       //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
    0x15, 0x00,       //   LOGICAL_MINIMUM (0)
    0x25, 0x01,       //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,       //   INPUT (Data,Var,Abs) ; Modifier byte
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x75, 0x08,       //   REPORT_SIZE (8)
    0x81, 0x03,       //   INPUT (Cnst,Var,Abs) ; Reserved byte
    0x95, 0x06,       //   REPORT_COUNT (6)
    0x75, 0x08,       //   REPORT_SIZE (8)
    0x15, 0x00,       //   LOGICAL_MINIMUM (0)
    0x25, 0x65,       //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,       //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,       //   USAGE_MINIMUM (Reserved (no event indicated))(0)
    0x29, 0x65,       //   USAGE_MAXIMUM (Keyboard Application)(101)
    0x81, 0x00,       //   INPUT (Data,Ary,Abs)
    0x95, 0x05,       //   REPORT_COUNT (5)
    0x75, 0x01,       //   REPORT_SIZE (1)
    0x05, 0x08,       //   USAGE_PAGE (LEDs)
    0x19, 0x01,       //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,       //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,       //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x75, 0x03,       //   REPORT_SIZE (3)
    0x91, 0x03,       //   OUTPUT (Cnst,Var,Abs)
    0xc0,             // END_COLLECTION
    0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
    0x09, 0x80,       // USAGE (System Control)
    0xa1, 0x01,       // COLLECTION (Application)
    0x85, 0x03,       //   REPORT_ID (3)
    0x15, 0x00,       //   LOGICAL_MINIMUM (0)
    0x25, 0x01,       //   LOGICAL_MAXIMUM (1)
    0x09, 0x82,       //   USAGE (System Sleep)
    0x75, 0x01,       //   REPORT_SIZE (1)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x81, 0x06,       //   INPUT (Data,Var,Rel)
    0x95, 0x07,       //   REPORT_COUNT (7)
    0x81, 0x03,       //   INPUT (Cnst,Var,Abs)
    0xc0              // END_COLLECTION
};

/** @defgroup USBD_HID_Private_Variables
  * @{
  */

USBD_ClassTypeDef USBD_HID = {
    USBD_HID_Init,
    USBD_HID_DeInit,
    USBD_HID_Setup,
    NULL, /*EP0_TxSent*/
    USBD_HID_EP0_RxReady,
    USBD_HID_DataIn, /*DataIn*/
    NULL,            /*DataOut*/
    NULL,            /*SOF */
    NULL,
    NULL,
    USBD_HID_GetCfgDesc,
    USBD_HID_GetCfgDesc,
    USBD_HID_GetCfgDesc,
    USBD_HID_GetDeviceQualifierDesc,
};

/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END = {
    0x09,                        /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    USB_HID_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01, /*bNumInterfaces: 1 interface*/
    0x01, /*bConfigurationValue: Configuration value*/
    0x00, /*iConfiguration: Index of string descriptor describing
  the configuration*/
    0xE0, /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32, /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x02,                    /*bInterfaceNumber: Number of Interface*/
    0x00,                    /*bAlternateSetting: Alternate setting*/
    0x01,                    /*bNumEndpoints*/
    0x03,                    /*bInterfaceClass: HID*/
    0x00,                    /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,                    /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    2,                       /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11,                /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,                    /*bCountryCode: Hardware target country*/
    0x01,                    /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,                    /*bDescriptorType*/
    HID_KB_REPORT_DESC_SIZE, /*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                   /*bLength: Endpoint Descriptor size*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/

    HID_EPIN_ADDR, /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    HID_EPIN_SIZE, /*wMaxPacketSize: 4 Byte max */
    0x00,
    HID_FS_BINTERVAL, /*bInterval: Polling Interval (10 ms)*/

};

/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __ALIGN_END = {
    /* 18 */
    0x09,                /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11,                /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,                    /*bCountryCode: Hardware target country*/
    0x01,                    /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,                    /*bDescriptorType : report descriptor type*/
    HID_KB_REPORT_DESC_SIZE, /*wItemLength: Total length of Report descriptor*/
    0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

void USBD_HID_Init_Sync()
{
    mUSBReadySem = xSemaphoreCreateBinary();
}

/** @defgroup USBD_HID_Private_Functions
  * @{
  */

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t USBD_HID_Init(USBD_HandleTypeDef* pdev, uint8_t cfgidx)
{
    uint8_t ret = 0;

    /* Open EP IN */
    USBD_LL_OpenEP(pdev, HID_EPIN_ADDR, USBD_EP_TYPE_INTR, HID_EPIN_SIZE);

    USBD_HID_Handle = &memHIDHandle;

    USBD_HID_Handle->state = HID_IDLE;

    return ret;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t USBD_HID_DeInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx)
{
    /* Close HID EPs */
    USBD_LL_CloseEP(pdev, HID_EPIN_ADDR);

    USBD_HID_Handle = NULL;

    return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
uint8_t USBD_HID_Setup(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req)
{
    uint16_t len = 0;
    uint8_t* pbuf = NULL;
    USBD_HID_HandleTypeDef* hhid = USBD_HID_Handle;

    ASSERT(hhid != NULL);

    switch(req->bmRequest & USB_REQ_TYPE_MASK)
    {
    case USB_REQ_TYPE_CLASS:
        switch(req->bRequest)
        {
        case HID_REQ_SET_PROTOCOL:
            hhid->Protocol = (uint8_t) (req->wValue);
            break;

        case HID_REQ_GET_PROTOCOL:
            USBD_CtlSendData(pdev, (uint8_t*) &hhid->Protocol, 1);
            break;

        case HID_REQ_SET_IDLE:
            hhid->IdleState = (uint8_t) (req->wValue >> 8);
            break;

        case HID_REQ_GET_IDLE:
            USBD_CtlSendData(pdev, (uint8_t*) &hhid->IdleState, 1);
            break;
        case HID_REQ_SET_REPORT:
            hhid->setReportAvailable = true;
            hhid->reportType = (uint8_t) (req->wValue >> 8);
            hhid->reportId = (uint8_t) (req->wValue);
            hhid->reportInterface = req->wIndex;
            USBD_CtlPrepareRx(pdev, hhid->reportBuffer, req->wLength);
            break;

        default:
            USBD_CtlError(pdev, req);
            return USBD_FAIL;
        }
        break;

    case USB_REQ_TYPE_STANDARD:
        switch(req->bRequest)
        {
        case USB_REQ_GET_DESCRIPTOR:
            if(req->wValue >> 8 == HID_REPORT_DESC)
            {
                len = MIN(HID_KB_REPORT_DESC_SIZE, req->wLength);
                pbuf = HID_KB_ReportDesc;
            }
            else if(req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
            {
                pbuf = USBD_HID_Desc;
                len = MIN(USB_HID_DESC_SIZ, req->wLength);
            }

            USBD_CtlSendData(pdev, pbuf, len); //Send data on EP0

            break;
        case HID_REQ_GET_REPORT:

            break;

        case USB_REQ_GET_INTERFACE:
            USBD_CtlSendData(pdev, (uint8_t*) &hhid->AltSetting, 1);
            break;

        case USB_REQ_SET_INTERFACE:
            hhid->AltSetting = (uint8_t) (req->wValue);
            break;
        }
    }
    return USBD_OK;
}

/**
  * @brief  USBD_HID_SendReport 
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef* pdev, uint8_t* report, uint16_t len)
{
    USBD_HID_HandleTypeDef* hhid = USBD_HID_Handle;

    if(hhid == NULL)
    {
        return USBD_FAIL;
    }

    if(pdev->dev_state == USBD_STATE_CONFIGURED)
    {
        if(hhid->state == HID_IDLE)
        {
            __disable_irq();
            hhid->state = HID_BUSY;
            USBD_StatusTypeDef ret = USBD_LL_Transmit(pdev, HID_EPIN_ADDR, report, len);
            __enable_irq();

            if(USBD_OK == ret)
            {
                // Wait for end of transfer
                xSemaphoreTake(mUSBReadySem, DELAY_SEND_REPORT_MS);
            }
        }
    }
    return USBD_OK;
}

/**
  * @brief  USBD_HID_GetPollingInterval 
  *         return polling interval from endpoint descriptor
  * @param  pdev: device instance
  * @retval polling interval
  */
uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef* pdev)
{
    uint32_t polling_interval = 0;

    /* HIGH-speed endpoints */
    if(pdev->dev_speed == USBD_SPEED_HIGH)
    {
        /* Sets the data transfer polling interval for high speed transfers. 
    Values between 1..16 are allowed. Values correspond to interval 
    of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL */
        polling_interval = (((1 << (HID_HS_BINTERVAL - 1))) / 8);
    }
    else /* LOW and FULL-speed endpoints */
    {
        /* Sets the data transfer polling interval for low and full 
    speed transfers */
        polling_interval = HID_FS_BINTERVAL;
    }

    return ((uint32_t) (polling_interval));
}

/**
  * @brief  USBD_HID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t* USBD_HID_GetCfgDesc(uint16_t* length)
{
    *length = sizeof(USBD_HID_CfgDesc);
    return USBD_HID_CfgDesc;
}


/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_HID_DataIn(USBD_HandleTypeDef* pdev, uint8_t epnum)
{
    BaseType_t taskWoken = pdFALSE;

    ASSERT(USBD_HID_Handle != NULL);

    /* Ensure that the FIFO is empty before a new transfer, this condition could 
  be caused by  a new transfer before the end of the previous transfer */
    USBD_HID_Handle->state = HID_IDLE;

    xSemaphoreGiveFromISR(mUSBReadySem, &taskWoken);
    portEND_SWITCHING_ISR(taskWoken);
    return USBD_OK;
}


/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_HID_GetDeviceQualifierDesc(uint16_t* length)
{
    *length = sizeof(USBD_HID_DeviceQualifierDesc);
    return USBD_HID_DeviceQualifierDesc;
}

/**
 * @brief  USBD_HID_RegisterInterface
   * @param  pdev: device instance
   * @param  itf: Interface callback
   * @retval status
   */
uint8_t USBD_HID_RegisterInterface(USBD_HandleTypeDef* pdev, USBD_HID_ItfTypeDef* itf)
{
    uint8_t ret = USBD_FAIL;

    if(itf != NULL)
    {
        pUSBD_HID_Interface = itf;
        ret = USBD_OK;
    }

    return ret;
}

/**
  * @brief  USBD_HID_EP0_RxReady
  *         Handles control request data.
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_HID_EP0_RxReady(void* pdev)
{
    USBD_HID_HandleTypeDef* hhid = USBD_HID_Handle;

    if(hhid->setReportAvailable)
    {
        uint16_t rxCount = USBD_GetRxCount(pdev, 0);
        hhid->setReportAvailable = false;

        ASSERT(pUSBD_HID_Interface != NULL);
        pUSBD_HID_Interface->SetReport(hhid->reportType,
                                       hhid->reportId,
                                       hhid->reportInterface,
                                       (uint8_t*) hhid->reportBuffer,
                                       rxCount);
    }
    return USBD_OK;
}
/**
  * @}
  */

/**
  * @}
  */


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
