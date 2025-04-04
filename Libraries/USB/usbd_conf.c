/**
  ******************************************************************************
  * @file           : usbd_conf.c
  * @brief          : This file implements the board support package for the USB device library
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
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_hid.h"
#include "usbPowerState.h"
#include "assertTools.h"
#include "timing_macros.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PCD_HandleTypeDef hpcd_USB_OTG_FS;
void Error_Handler(void);

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
/* Exported function prototypes -----------------------------------------------*/
extern USBD_StatusTypeDef USBD_LL_BatteryCharging(USBD_HandleTypeDef* pdev);
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN 1 */
static void SystemClockConfig_Resume(void);
/* USER CODE END 1 */
void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef* hpcd, uint8_t state);
extern void SystemClock_Config(void);

bool resumeUSB()
{
    bool ret = false;

    USBD_HandleTypeDef* pDev = (USBD_HandleTypeDef*) hpcd_USB_OTG_FS.pData;
    if(pDev->dev_remote_wakeup == 1 && pDev->dev_state == USBD_STATE_SUSPENDED)
    {
        HAL_PCD_ResumeCallback(&hpcd_USB_OTG_FS);
        TRACE_FUNC_ACTION("Activate remote wakeup\n");

        /* Activate Remote wakeup */
        if(HAL_OK == HAL_PCD_ActivateRemoteWakeup(&hpcd_USB_OTG_FS))
        {
            /* Remote wakeup delay */
            HAL_Delay(5);
            TRACE_FUNC_ACTION("Deactivate remote wakeup\n");
            /* Disable Remote wakeup */
            if(HAL_OK == HAL_PCD_DeActivateRemoteWakeup(&hpcd_USB_OTG_FS))
            {
                ret = true;
            }
        }
    }

    return ret;
}

/*******************************************************************************
                       LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/
/* MSP Init */

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(pcdHandle->Instance == USB_OTG_FS)
    {
        /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

        /* USER CODE END USB_OTG_FS_MspInit 0 */

        /**USB_OTG_FS GPIO Configuration    
    PA9     ------> USB_OTG_FS_VBUS
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP 
    */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

        /* Enable VDDUSB */
        if(__HAL_RCC_PWR_IS_CLK_DISABLED())
        {
            __HAL_RCC_PWR_CLK_ENABLE();
            HAL_PWREx_EnableVddUSB();
            __HAL_RCC_PWR_CLK_DISABLE();
        }
        else
        {
            HAL_PWREx_EnableVddUSB();
        }

        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
        /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

        /* USER CODE END USB_OTG_FS_MspInit 1 */
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
    if(pcdHandle->Instance == USB_OTG_FS)
    {
        /* USER CODE BEGIN USB_OTG_FS_MspDeInit 0 */

        /* USER CODE END USB_OTG_FS_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

        /**USB_OTG_FS GPIO Configuration    
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP 
    */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /* Disable VDDUSB */
        if(__HAL_RCC_PWR_IS_CLK_DISABLED())
        {
            __HAL_RCC_PWR_CLK_ENABLE();
            HAL_PWREx_DisableVddUSB();
            __HAL_RCC_PWR_CLK_DISABLE();
        }
        else
        {
            HAL_PWREx_DisableVddUSB();
        }

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(OTG_FS_IRQn);

        /* USER CODE BEGIN USB_OTG_FS_MspDeInit 1 */

        /* USER CODE END USB_OTG_FS_MspDeInit 1 */
    }
}

/**
  * @brief  Setup Stage callback
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef* hpcd)
{
    USBD_LL_SetupStage((USBD_HandleTypeDef*) hpcd->pData, (uint8_t*) hpcd->Setup);
}

/**
  * @brief  Data Out Stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    USBD_LL_DataOutStage((USBD_HandleTypeDef*) hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
  * @brief  Data In Stage callback..
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    USBD_LL_DataInStage((USBD_HandleTypeDef*) hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
  * @brief  SOF callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef* hpcd)
{
    USBD_LL_SOF((USBD_HandleTypeDef*) hpcd->pData);
}

/**
  * @brief  Reset callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef* hpcd)
{
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

    /*Set USB Current Speed*/
    switch(hpcd->Init.speed)
    {
    case PCD_SPEED_FULL:
    default:
        speed = USBD_SPEED_FULL;
        break;
    }
    USBD_LL_SetSpeed((USBD_HandleTypeDef*) hpcd->pData, speed);

    /*Reset Device*/
    USBD_LL_Reset((USBD_HandleTypeDef*) hpcd->pData);
}

/**
  * @brief  Suspend callback.
  * When Low power mode is enabled the debug cannot be used (IAR, Keil doesn't support it)
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef* hpcd)
{
    __HAL_PCD_GATE_PHYCLOCK(hpcd);
    /* Inform USB library that core enters in suspend Mode */
    USBD_LL_Suspend((USBD_HandleTypeDef*) hpcd->pData);
    /*Enter in STOP mode */
    /* USER CODE BEGIN 2 */
    if(hpcd->Init.low_power_enable)
    {
        /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
        SCB->SCR |= (uint32_t) ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
    }

    notifyUsbSuspend();

    /* USER CODE END 2 */
}

/**
  * @brief  Resume callback.
  * When Low power mode is enabled the debug cannot be used (IAR, Keil doesn't support it)
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef* hpcd)
{
    __HAL_PCD_UNGATE_PHYCLOCK(hpcd);
    /* USER CODE BEGIN 3 */
    if(hpcd->Init.low_power_enable)
    {
        /* Reset SLEEPDEEP bit of Cortex System Control Register */
        SCB->SCR &= (uint32_t) ~((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
        SystemClockConfig_Resume();
    }

    notifyUsbResume();

    /* USER CODE END 3 */
    USBD_LL_Resume((USBD_HandleTypeDef*) hpcd->pData);
}

/**
  * @brief  ISOOUTIncomplete callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef*) hpcd->pData, epnum);
}

/**
  * @brief  ISOINIncomplete callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef* hpcd, uint8_t epnum)
{
    USBD_LL_IsoINIncomplete((USBD_HandleTypeDef*) hpcd->pData, epnum);
}

/**
  * @brief  ConnectCallback callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef* hpcd)
{
    USBD_LL_DevConnected((USBD_HandleTypeDef*) hpcd->pData);

    notifyUsbConnect();
}

/**
  * @brief  Disconnect callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef* hpcd)
{
    USBD_LL_DevDisconnected((USBD_HandleTypeDef*) hpcd->pData);

    notifyUsbDisconnect();
}

/*******************************************************************************
                       LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/
/**
  * @brief  Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef* pdev)
{
    /* Init USB_IP */
    if(pdev->id == DEVICE_FS)
    {
        /* enable USB power on Pwrctrl CR2 register */
        /* Link The driver to the stack */
        hpcd_USB_OTG_FS.pData = pdev;
        pdev->pData = &hpcd_USB_OTG_FS;

        hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
        hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
        hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
        hpcd_USB_OTG_FS.Init.ep0_mps = DEP0CTL_MPS_64;
        hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
        hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
        hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
        hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
        hpcd_USB_OTG_FS.Init.battery_charging_enable = DISABLE;
        hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
        if(HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        // 0 is IN and out
        HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS, 64);
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 64);
        // 1 is CMD IN from CDC ACM
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 8);
        // 2 is bulk in from CDC_ACM
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 2, 64);
        // 4 is Interrupt from HID
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 4, 16);
    }
    return USBD_OK;
}

/**
  * @brief  De-Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef* pdev)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_DeInit(pdev->pData);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Starts the Low Level portion of the Device driver. 
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef* pdev)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_Start(pdev->pData);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Stops the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef* pdev)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_Stop(pdev->pData);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Opens an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  ep_type: Endpoint Type
  * @param  ep_mps: Endpoint Max Packet Size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef* pdev,
                                  uint8_t ep_addr,
                                  uint8_t ep_type,
                                  uint16_t ep_mps)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);


    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Closes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_Close(pdev->pData, ep_addr);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Flushes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_Flush(pdev->pData, ep_addr);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_SetStall(pdev->pData, ep_addr);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Returns Stall condition.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Stall (1: Yes, 0: No)
  */
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
    PCD_HandleTypeDef* hpcd = (PCD_HandleTypeDef*) pdev->pData;

    if((ep_addr & 0x80) == 0x80)
    {
        return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
    }
    else
    {
        return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
    }
}
/**
  * @brief  Assigns a USB address to the device.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef* pdev, uint8_t dev_addr)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_SetAddress(pdev->pData, dev_addr);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Transmits data over an endpoint.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be sent
  * @param  size: Data size    
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef* pdev,
                                    uint8_t ep_addr,
                                    uint8_t* pbuf,
                                    uint16_t size)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Prepares an endpoint for reception.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be received
  * @param  size: Data size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef* pdev,
                                          uint8_t ep_addr,
                                          uint8_t* pbuf,
                                          uint16_t size)
{
    HAL_StatusTypeDef hal_status = HAL_OK;
    USBD_StatusTypeDef usb_status = USBD_OK;

    hal_status = HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);

    switch(hal_status)
    {
    case HAL_OK:
        usb_status = USBD_OK;
        break;
    case HAL_ERROR:
        usb_status = USBD_FAIL;
        break;
    case HAL_BUSY:
        usb_status = USBD_BUSY;
        break;
    case HAL_TIMEOUT:
        usb_status = USBD_FAIL;
        break;
    default:
        usb_status = USBD_FAIL;
        break;
    }
    return usb_status;
}

/**
  * @brief  Returns the last transfered packet size.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Recived Data Size
  */
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef* pdev, uint8_t ep_addr)
{
    return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef*) pdev->pData, ep_addr);
}
/**
  * @brief  GPIO EXTI Callback function
  *         Handle USB VBUS detection upon External interrupt
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Callback_USB(uint16_t GPIO_Pin)
{
    // TODO : use GPIO class !
    if(GPIO_Pin == GPIO_PIN_9)
    {
        HAL_PCDEx_BCD_VBUSDetect(&hpcd_USB_OTG_FS);
    }
}

#if(USBD_LPM_ENABLED == 1)
/**
  * @brief  HAL_PCDEx_LPM_Callback : Send LPM message to user layer
  * @param  hpcd: PCD handle
  * @param  msg: LPM message
  * @retval HAL status
  */
void HAL_PCDEx_LPM_Callback(PCD_HandleTypeDef* hpcd, PCD_LPM_MsgTypeDef msg)
{
    switch(msg)
    {
    case PCD_LPM_L0_ACTIVE:
        if(hpcd->Init.low_power_enable)
        {
            SystemClock_Config();

            /* Reset SLEEPDEEP bit of Cortex System Control Register */
            SCB->SCR &= (uint32_t) ~((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
        }
        __HAL_PCD_UNGATE_PHYCLOCK(hpcd);
        USBD_LL_Resume(hpcd->pData);
        break;

    case PCD_LPM_L1_ACTIVE:
        __HAL_PCD_GATE_PHYCLOCK(hpcd);
        USBD_LL_Suspend(hpcd->pData);

        /*Enter in STOP mode */
        if(hpcd->Init.low_power_enable)
        {
            /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
            SCB->SCR |= (uint32_t) ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
        }
        break;
    }
}
#endif
/**
  * @brief  Delays routine for the USB Device Library.
  * @param  Delay: Delay in ms
  * @retval None
  */
void USBD_LL_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}

/**
  * @brief  static single allocation.
  * @param  size: size of allocated memory
  * @retval None
  */
void* USBD_static_malloc(uint32_t size)
{
    ASSERT(false);
    return NULL;
}

/**
  * @brief  Dummy memory free
  * @param  *p pointer to allocated  memory address
  * @retval None
  */
void USBD_static_free(void* p)
{
}

/* USER CODE BEGIN 5 */
/**
  * @brief  Configures system clock after wake-up from USB Resume CallBack: 
  *         enable HSI, PLL and select PLL as system clock source.
  * @param  None
  * @retval None
  */
static void SystemClockConfig_Resume(void)
{
    SystemClock_Config();
}
/* USER CODE END 5 */

/**
* @brief Software Device Connection
* @param hpcd: PCD handle
* @param state: connection state (0 : disconnected / 1: connected) 
* @retval None
*/
void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef* hpcd, uint8_t state)
{
    /* USER CODE BEGIN 6 */
    if(state == 1)
    {
        /* Configure Low Connection State */
    }
    else
    {
        /* Configure High Connection State */
    }
    /* USER CODE END 6 */
}

/**
  * @brief  Verify if the Battery Charging Detection mode (BCD) is used :
  *         return USBD_OK if true
  *         else return USBD_FAIL if false
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_BatteryCharging(USBD_HandleTypeDef* pdev)
{
    PCD_HandleTypeDef* hpcd = (PCD_HandleTypeDef*) pdev->pData;
    if(hpcd->Init.battery_charging_enable == ENABLE)
    {
        return USBD_OK;
    }
    else
    {
        return USBD_FAIL;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
