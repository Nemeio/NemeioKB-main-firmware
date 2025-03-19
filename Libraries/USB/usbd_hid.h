/**
  ******************************************************************************
  * @file    usbd_hid.h
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   Header file for the usbd_hid_core.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HID_H
#define __USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_ioreq.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_HID
  * @brief This file is the Header file for usbd_hid.c
  * @{
  */


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */
#define HID_EPIN_ADDR 0x84
#define HID_EPIN_SIZE 0x09 //0x08
#define HID_EPIN5_ADDR 0x85

#define HID_EPOUT_ADDR 0x05
#define HID_EPOUT_SIZE 0x40

#define USB_HID_CONFIG_DESC_SIZ 0x3B //0x29 /*41*/
#define USB_HID_DESC_SIZ 9
#define HID_KB_REPORT_01_DESC_SIZE 0x2D

#define HID_KB_REPORT_DESC_SIZE 128
extern __ALIGN_BEGIN uint8_t HID_KB_ReportDesc[HID_KB_REPORT_DESC_SIZE];

#define HID_DESCRIPTOR_TYPE 0x21
#define HID_REPORT_DESC 0x22

#define HID_HS_BINTERVAL 0x07
#define HID_FS_BINTERVAL 0x04 // 0x0A
#define HID_POLLING_INTERVAL 0x0A

#define HID_REQ_SET_PROTOCOL 0x0B
#define HID_REQ_GET_PROTOCOL 0x03

#define HID_REQ_SET_IDLE 0x0A
#define HID_REQ_GET_IDLE 0x02

#define HID_REQ_SET_REPORT 0x09
#define HID_REQ_GET_REPORT 0x01
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef enum
{
    HID_IDLE = 0,
    HID_BUSY,
} HID_StateTypeDef;

typedef struct _USBD_HID_Itf
{
    void (*SetReport)(uint8_t, uint8_t, uint16_t, const uint8_t*, uint16_t);
} USBD_HID_ItfTypeDef;

typedef struct
{
    uint32_t reportBuffer[HID_EPOUT_SIZE / sizeof(uint32_t)]; /* Force 32bits alignment */
    uint32_t Protocol;
    uint32_t IdleState;
    uint32_t AltSetting;
    HID_StateTypeDef state;
    bool setReportAvailable;
    uint8_t reportType;
    uint8_t reportId;
    uint16_t reportInterface;
} USBD_HID_HandleTypeDef;
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_HID;
#define USBD_HID_CLASS &USBD_HID
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
void USBD_HID_Init_Sync();

uint8_t USBD_HID_SendReport(USBD_HandleTypeDef* pdev, uint8_t* report, uint16_t len);

uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef* pdev);

uint8_t USBD_HID_RegisterInterface(USBD_HandleTypeDef* pdev, USBD_HID_ItfTypeDef* itf);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_HID_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
