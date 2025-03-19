/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @brief          :
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
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
#include "usbd_cdc_if.h"
/* USER CODE BEGIN INCLUDE */
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "cmsis_os.h"
#include "usbd_custom_composite.h"
#include "assertTools.h"
#include <stdio.h>
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CDC 
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_CDC_Private_Defines
  * @{
  */
/* USER CODE BEGIN PRIVATE_DEFINES */
/* Define size for the receive and transmit buffer over CDC */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE CDC_DATA_FS_MAX_PACKET_SIZE
#define APP_TX_DATA_SIZE CDC_DATA_FS_MAX_PACKET_SIZE

#define CDC_TASK_STACK_SIZE 256
#define CDC_RECEIVE_DATA_TASK_PRIORITY osPriorityNormal
#define CDC_TRANSMIT_DATA_TASK_PRIORITY osPriorityHigh

#define CDC_TRANSMIT_DELAY_MS 10

#define CDC_LINE_CODING_SZ 7

#define CDC_CTRL_SIG_BITMAP_DTE_IDX 0
#define CDC_CTRL_SIG_BITMAP_DTE_EN_FLAG (0x1U << CDC_CTRL_SIG_BITMAP_DTE_IDX)
/* USER CODE END PRIVATE_DEFINES */
/**
  * @}
  */

/** @defgroup USBD_CDC_Private_TypesDefinitions
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
typedef struct _CDCInPacket_t
{
    uint16_t len;
    uint8_t buff[APP_RX_DATA_SIZE];
} CDCInPacket_t;

typedef struct _CDCOutPacket_t
{
    uint16_t len;
    uint8_t buff[APP_TX_DATA_SIZE];
} CDCOutPacket_t;

typedef struct _CDCInListener_t
{
    void* pListener;
    uint8_t (*DataReceived)(void* pListener, const uint8_t* pData, size_t len);
} CDCInListener_t;
/* USER CODE END PRIVATE_TYPES */
/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Macros
  * @{
  */
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Variables
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/* Received Data over USB are stored in this buffer       */
__ALIGN_BEGIN uint8_t UserRxBufferFS[APP_RX_DATA_SIZE] __ALIGN_END;

/* USER CODE BEGIN PRIVATE_VARIABLES */

static StaticTask_t CDCReceiveDataTCBBuffer;
static StackType_t CDCReceiveDataStack[CDC_TASK_STACK_SIZE];
static StaticTask_t CDCTransmitDataTCBBuffer;
static StackType_t CDCTransmitDataStack[CDC_TASK_STACK_SIZE];

static xSemaphoreHandle TXSemaphore;
static QueueHandle_t CDCInputQueue = {0};
static QueueHandle_t CDCOutputQueue = {0};

static CDCInListener_t CDCInListener = {NULL, NULL};

static USBD_HandleTypeDef* pUsbDevice;

static bool mbDTREnabled = false;
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables
  * @{
  */
extern USBD_CDC_HandleTypeDef* USBD_CDC_Handle;
/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */
static int8_t CDC_Init_FS(USBD_HandleTypeDef* pdev);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(USBD_HandleTypeDef* pdev, uint8_t* pbuf, uint32_t* Len);
static int8_t CDC_DataSent_FS(void);

static void CDC_UpdateDTRState(uint16_t ctlSigBitmap);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_CDC_Interface_fops_FS = {CDC_Init_FS,
                                                  CDC_DeInit_FS,
                                                  CDC_Control_FS,
                                                  CDC_Receive_FS,
                                                  CDC_DataSent_FS};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CDC_Init_FS
  *         Initializes the CDC media low layer over the FS USB IP
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(USBD_HandleTypeDef* pdev)
{
    /* USER CODE BEGIN 3 */
    /* Set Application Buffers */
    // TX Buffer will be set by CDC_Transmit_FS
    USBD_CDC_SetRxBuffer(pdev, UserRxBufferFS);
    return (USBD_OK);
    /* USER CODE END 3 */
}

/**
  * @brief  CDC_DeInit_FS
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
    /* USER CODE BEGIN 4 */
    return (USBD_OK);
    /* USER CODE END 4 */
}

/**
  * @brief  CDC_Control_FS
  *         Manage the CDC class requests
  * @param  cmd: Command code            
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
    /* USER CODE BEGIN 5 */
    static uint8_t lineCoding[CDC_LINE_CODING_SZ] = {0};

    switch(cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:

        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

        break;

    case CDC_SET_COMM_FEATURE:

        break;

    case CDC_GET_COMM_FEATURE:

        break;

    case CDC_CLEAR_COMM_FEATURE:

        break;

    /*******************************************************************************/
    /* Line Coding Structure                                                       */
    /*-----------------------------------------------------------------------------*/
    /* Offset | Field       | Size | Value  | Description                          */
    /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
    /* 4      | bCharFormat |   1  | Number | Stop bits                            */
    /*                                        0 - 1 Stop bit                       */
    /*                                        1 - 1.5 Stop bits                    */
    /*                                        2 - 2 Stop bits                      */
    /* 5      | bParityType |  1   | Number | Parity                               */
    /*                                        0 - None                             */
    /*                                        1 - Odd                              */
    /*                                        2 - Even                             */
    /*                                        3 - Mark                             */
    /*                                        4 - Space                            */
    /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
    /*******************************************************************************/
    case CDC_SET_LINE_CODING:
        if(length >= sizeof(lineCoding))
        {
            memcpy(lineCoding, pbuf, sizeof(lineCoding));
        }
        break;

    case CDC_GET_LINE_CODING:
        if(length >= sizeof(lineCoding))
        {
            memcpy(pbuf, lineCoding, sizeof(lineCoding));
        }
        break;

    case CDC_SET_CONTROL_LINE_STATE:
    {
        USBD_SetupReqTypedef* pReq = (USBD_SetupReqTypedef*) pbuf;
        if(pReq != NULL)
        {
            CDC_UpdateDTRState(pReq->wValue);
        }
    }
    break;

    case CDC_SEND_BREAK:

        break;

    default:
        break;
    }

    return (USBD_OK);
    /* USER CODE END 5 */
}

/**
  * @brief  CDC_Receive_FS
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(USBD_HandleTypeDef* pdev, uint8_t* Buf, uint32_t* Len)
{
    /* USER CODE BEGIN 6 */
    BaseType_t taskWoken = pdFALSE;
    CDCInPacket_t buffTmpInput = {0};

    // CDC_In_Packet buffer length is defined as max USB packet size so Len value is OK
    buffTmpInput.len = *Len;
    memcpy(&buffTmpInput.buff, Buf, *Len);

    USBD_CDC_SetRxBuffer(pdev, &Buf[0]);
    USBD_CDC_ReceivePacket(pdev);

    xQueueSendFromISR(CDCInputQueue, &buffTmpInput, &taskWoken);

    portEND_SWITCHING_ISR(taskWoken);
    return (USBD_OK);
    /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data send over USB IN endpoint are sent over CDC interface 
  *         through this function.           
  *         @note
  *         
  *                 
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(USBD_HandleTypeDef* pdev, uint8_t* Buf, uint16_t Len)
{
    uint8_t result = USBD_FAIL;
    /* USER CODE BEGIN 7 */
    USBD_CDC_HandleTypeDef* hcdc = USBD_CDC_Handle;

    if(hcdc == NULL)
    {
        return USBD_FAIL;
    }

    if(hcdc->TxState != 0)
    {
        return USBD_BUSY;
    }

    USBD_CDC_SetTxBuffer(pdev, Buf, Len);

    __disable_irq();
    if(mbDTREnabled)
    {
        // Make sure the semaphore is empty before starting transfer
        xSemaphoreTake(TXSemaphore, 0);
        result = USBD_CDC_TransmitPacket(pdev);
    }
    __enable_irq();

    // Wait for previous packet to be sent (call to DataIN)
    if(USBD_OK == result)
    {
        if(xSemaphoreTake(TXSemaphore, portMAX_DELAY) != pdTRUE)
        {
            result = USBD_FAIL;
        }
    }
    /* USER CODE END 7 */
    return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @brief  CDC_DataSent_FS
  *         Notification that data have been sent over USB OUT endpoint
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
static int8_t CDC_DataSent_FS(void)
{
    uint8_t result = USBD_OK;
    /* USER CODE BEGIN 7 */
    BaseType_t taskWoken = pdFALSE;

    xSemaphoreGiveFromISR(TXSemaphore, &taskWoken);
    portEND_SWITCHING_ISR(taskWoken);
    /* USER CODE END 7 */
    return result;
}

static void CDC_UpdateDTRState(uint16_t ctlSigBitmap)
{
    BaseType_t taskWoken = pdFALSE;

    mbDTREnabled = (ctlSigBitmap & CDC_CTRL_SIG_BITMAP_DTE_EN_FLAG);

    if(!mbDTREnabled)
    {
        // Cancel the eventual transfer in progress
        // xSemaphoreGiveFromISR may return pdFALSE if no transfer is in progress
        xSemaphoreGiveFromISR(TXSemaphore, &taskWoken);
        portEND_SWITCHING_ISR(taskWoken);
    }
}

static void CDCReceiveDataTask(void* pvParameters)
{
    for(;;)
    {
        static CDCInPacket_t bufferTmpInput = {0};
        xQueueReceive(CDCInputQueue, &bufferTmpInput, portMAX_DELAY);

        if(CDCInListener.DataReceived != NULL)
        {
            CDCInListener.DataReceived(CDCInListener.pListener,
                                       bufferTmpInput.buff,
                                       bufferTmpInput.len);
        }
    }
}

static void CDCTransmitDataTask(void* pvParameters)
{
    for(;;)
    {
        static CDCOutPacket_t bufferTmpOutput = {0};
        if(pdTRUE == xQueueReceive(CDCOutputQueue, &bufferTmpOutput, portMAX_DELAY))
        {
            USBD_CUSTOM_ACM_HID_CDC_Send(pUsbDevice, bufferTmpOutput.buff, bufferTmpOutput.len);
        }
#ifdef DEBUG
        else
        {
            asm("bkpt #0");
        }
#endif
    }
}

void CDC_InitInterface(USBD_HandleTypeDef* pdev,
                       void* pListener,
                       uint8_t (*dataReceivedCallback)(void*, const uint8_t*, size_t))
{
    TXSemaphore = xSemaphoreCreateBinary();
    ASSERT(TXSemaphore != NULL);

    pUsbDevice = pdev;

    CDCInputQueue = xQueueCreate(150, sizeof(CDCInPacket_t));
    ASSERT(CDCInputQueue != NULL);

    CDCOutputQueue = xQueueCreate(150, sizeof(CDCOutPacket_t));
    ASSERT(CDCOutputQueue != NULL);

    osThreadId threadIdRet = NULL;

    osThreadStaticDef(CDCReceiveDataTask,
                      reinterpret_cast<os_pthread>(CDCReceiveDataTask),
                      CDC_RECEIVE_DATA_TASK_PRIORITY,
                      0,
                      CDC_TASK_STACK_SIZE,
                      CDCReceiveDataStack,
                      &CDCReceiveDataTCBBuffer);
    threadIdRet = osThreadCreate(osThread(CDCReceiveDataTask), NULL);
    ASSERT(threadIdRet != NULL);

    osThreadStaticDef(CDCTransmitDataTask,
                      reinterpret_cast<os_pthread>(CDCTransmitDataTask),
                      CDC_TRANSMIT_DATA_TASK_PRIORITY,
                      0,
                      CDC_TASK_STACK_SIZE,
                      CDCTransmitDataStack,
                      &CDCTransmitDataTCBBuffer);
    threadIdRet = osThreadCreate(osThread(CDCTransmitDataTask), NULL);
    ASSERT(threadIdRet != NULL);

    CDCInListener.pListener = pListener;
    CDCInListener.DataReceived = dataReceivedCallback;
}

void CDC_SendData(const uint8_t* pData, size_t len)
{
    CDCOutPacket_t bufferTmpOutput = {0};
    uint16_t remainingDataLength = len;

    if(pData != NULL)
    {
        while(remainingDataLength)
        {
            uint16_t copyLen = remainingDataLength;

            if(copyLen > sizeof(bufferTmpOutput.buff))
            {
                copyLen = sizeof(bufferTmpOutput.buff);
            }

            bufferTmpOutput.len = copyLen;
            memcpy(bufferTmpOutput.buff, pData, bufferTmpOutput.len);

            if(xQueueSend(CDCOutputQueue, &bufferTmpOutput, CDC_TRANSMIT_DELAY_MS) != pdPASS)
            {
#ifdef DEBUG
                __asm("bkpt #0");
#endif
            }

            // If last packet length is equal to max USB packet size, send an empty packet
            if(remainingDataLength == sizeof(bufferTmpOutput.buff))
            {
                bufferTmpOutput.len = 0;
                if(xQueueSend(CDCOutputQueue, &bufferTmpOutput, CDC_TRANSMIT_DELAY_MS) != pdPASS)
                {
#ifdef DEBUG
                    __asm("bkpt #0");
#endif
                }

                remainingDataLength = 0;
            }
            else
            {
                remainingDataLength -= bufferTmpOutput.len;
                pData += bufferTmpOutput.len;
            }
        }
    }
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
