/**
  ******************************************************************************
  * @file           : usbd_desc.c
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device descriptors
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "assertTools.h"

#include "Version.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_DESC 
  * @brief USBD descriptors module
  * @{
  */

/** @defgroup USBD_DESC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Defines
  * @{
  */
#define USBD_VID 0x3600
#define USBD_LANGID_STRING 1033
#define USBD_MANUFACTURER_STRING "Groupe LDLC"
#define USBD_PID_FS 0x0002
#define USBD_PRODUCT_STRING_FS "Nemeio Keypad"
#define USBD_CONFIGURATION_STRING_FS "Nemeio Keypad Config"
#define USBD_INTERFACE_STRING_FS "Nemeio Keypad Interface"
/* Warning these are BCD values : 2 x 4 bits (99 = 9 << 4 | 9)
* static_assert(((USBD_SERIAL_PROTOCOL_VERSION & 0xF) <= 0x9) && ((USBD_SERIAL_PROTOCOL_VERSION >> 4) & 0xF) <= 0x9)
* static_assert(((USBD_HARDWARE_VERSION & 0xF) <= 0x9) && ((USBD_HARDWARE_VERSION >> 4) & 0xF) <= 0x9)
*/

#define USBD_SERIAL_PROTOCOL_VERSION PROTOCOL_VERSION
#define USBD_HARDWARE_VERSION HARDWARE_REVISION

#define USB_SIZ_BOS_DESC 0x0C

/* USER CODE BEGIN 0 */

/* USER CODE END 0*/
/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Variables
  * @{
  */
uint8_t* USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);

#ifdef USB_SUPPORT_USER_STRING_DESC
uint8_t* USBD_FS_USRStringDesc(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t* length);
#endif /* USB_SUPPORT_USER_STRING_DESC */

#if(USBD_LPM_ENABLED == 1)
uint8_t* USBD_FS_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
#endif

USBD_DescriptorsTypeDef FS_Desc = {
    USBD_FS_DeviceDescriptor,
    USBD_FS_LangIDStrDescriptor,
    USBD_FS_ManufacturerStrDescriptor,
    USBD_FS_ProductStrDescriptor,
    USBD_FS_SerialStrDescriptor,
    USBD_FS_ConfigStrDescriptor,
    USBD_FS_InterfaceStrDescriptor,
#if(USBD_LPM_ENABLED == 1)
    USBD_FS_USR_BOSDescriptor,
#endif
};

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc_CDC[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12,                 /*bLength */
    USB_DESC_TYPE_DEVICE, /*bDescriptorType*/
#if(USBD_LPM_ENABLED == 1)
    0x01,
/*bcdUSB */ /* changed to USB version 2.01
                                             in order to support LPM L1 suspend
                                             resume test of USBCV3.0*/
#else
    0x00, /*bcdUSB */
#endif /* (USBD_LPM_ENABLED == 1) */
    0x02,
    0x02,                         /*bDeviceClass*/
    0x02,                         /*bDeviceSubClass*/
    0x00,                         /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,             /*bMaxPacketSize*/
    LOBYTE(USBD_VID),             /*idVendor*/
    HIBYTE(USBD_VID),             /*idVendor*/
    LOBYTE(USBD_PID_FS),          /*idProduct*/
    HIBYTE(USBD_PID_FS),          /*idProduct*/
    USBD_SERIAL_PROTOCOL_VERSION, /*bcdDevice */
    USBD_HARDWARE_VERSION,
    USBD_IDX_MFC_STR,          /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,      /*Index of product string*/
    USBD_IDX_SERIAL_STR,       /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION /*bNumConfigurations*/
};


/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12,                 /*bLength */
    USB_DESC_TYPE_DEVICE, /*bDescriptorType*/
#if(USBD_LPM_ENABLED == 1)
    0x01,
/*bcdUSB */ /* changed to USB version 2.01
                                               in order to support LPM L1 suspend
                                               resume test of USBCV3.0*/
#else
    0x00, /* bcdUSB */
#endif
    0x02,                         /*bcdUSB*/
    0xEF,                         /*bDeviceClass: Multi Interface*/
    0x02,                         /*bDeviceSubClass: Common Class*/
    0x01,                         /*bDeviceProtocol: Interface Association Descriptor*/
    USB_MAX_EP0_SIZE,             /*bMaxPacketSize*/
    LOBYTE(USBD_VID),             /*idVendor*/
    HIBYTE(USBD_VID),             /*idVendor*/
    LOBYTE(USBD_PID_FS),          /*idVendor*/
    HIBYTE(USBD_PID_FS),          /*idVendor*/
    USBD_SERIAL_PROTOCOL_VERSION, /*bcdDevice */
    USBD_HARDWARE_VERSION,
    USBD_IDX_MFC_STR,          /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,      /*Index of product string*/
    USBD_IDX_SERIAL_STR,       /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION /*bNumConfigurations*/
};

/* USB_DeviceDescriptor */
/* BOS descriptor */
#if(USBD_LPM_ENABLED == 1)
#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN uint8_t
    USBD_FS_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END = {0x5,
                                                     USB_DESC_TYPE_BOS,
                                                     0xC,
                                                     0x0,
                                                     0x1, /* 1 device capability */
                                                          /* device capability*/
                                                     0x7,
                                                     USB_DEVICE_CAPABITY_TYPE,
                                                     0x2,
                                                     0x2, /*LPM capability bit set */
                                                     0x0,
                                                     0x0,
                                                     0x0};
#endif

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
    USB_LEN_LANGID_STR_DESC,
    USB_DESC_TYPE_STRING,
    LOBYTE(USBD_LANGID_STRING),
    HIBYTE(USBD_LANGID_STRING),
};

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#define MAX_SERIAL_NUMBER_ASCII_STR_SIZ 25
static char USBD_SerialNumberAsciiStr[MAX_SERIAL_NUMBER_ASCII_STR_SIZ] =
    USBD_SERIALNUMBER_DEFAULT_STRING_FS;
static enum e_USBProfile mUsbProfile = STD_PROFILE;

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Functions
  * @{
  */

/**
* @brief  USBD_FS_DeviceDescriptor 
*         return the device descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    uint8_t* profile = NULL;
    switch(mUsbProfile)
    {
    case STD_PROFILE:
        *length = sizeof(USBD_FS_DeviceDesc);
        profile = USBD_FS_DeviceDesc;
        break;
    case CDC_PROFILE:
        *length = sizeof(USBD_FS_DeviceDesc_CDC);
        profile = USBD_FS_DeviceDesc_CDC;
        break;
    default:
        ASSERT(false);
    }

    return profile;
}

/**
* @brief  USBD_FS_LangIDStrDescriptor 
*         return the LangID string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof(USBD_LangIDDesc);
    return USBD_LangIDDesc;
}

/**
* @brief  USBD_FS_ProductStrDescriptor 
*         return the product string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString(USBD_PRODUCT_STRING_FS, USBD_StrDesc, length);

    return USBD_StrDesc;
}

/**
* @brief  USBD_FS_ManufacturerStrDescriptor 
*         return the manufacturer string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString(USBD_MANUFACTURER_STRING, USBD_StrDesc, length);

    return USBD_StrDesc;
}

/**
* @brief  USBD_FS_SerialStrDescriptor 
*         return the serial number string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString((uint8_t*) USBD_SerialNumberAsciiStr, USBD_StrDesc, length);

    return USBD_StrDesc;
}

void USBD_FS_Init(const char* serialNumber, enum e_USBProfile usbProfile)
{
    if(serialNumber != NULL)
    {
        strncpy(USBD_SerialNumberAsciiStr, serialNumber, sizeof(USBD_SerialNumberAsciiStr));
        USBD_SerialNumberAsciiStr[sizeof(USBD_SerialNumberAsciiStr) - 1] = '\0';
    }

    mUsbProfile = usbProfile;
}

/**
* @brief  USBD_FS_ConfigStrDescriptor 
*         return the configuration string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString(USBD_CONFIGURATION_STRING_FS, USBD_StrDesc, length);

    return USBD_StrDesc;
}

/**
* @brief  USBD_HS_InterfaceStrDescriptor 
*         return the interface string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t* USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString(USBD_INTERFACE_STRING_FS, USBD_StrDesc, length);

    return USBD_StrDesc;
}
#if(USBD_LPM_ENABLED == 1)
/**
  * @brief  USBD_FS_USR_BOSDescriptor 
  *         return the BOS descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t* USBD_FS_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof(USBD_FS_BOSDesc);

    return (uint8_t*) USBD_FS_BOSDesc;
}
#endif
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
