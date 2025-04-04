/**
  ******************************************************************************
  * File Name          : QUADSPI.c
  * Description        : This file provides code for the configuration
  *                      of the QUADSPI instances.
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
#include "quadspi.h"

#include "gpio.h"
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;
DMA_HandleTypeDef hdma_quadspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{
    hqspi.Instance = QUADSPI;
    hqspi.Init.ClockPrescaler = 255;
    hqspi.Init.FifoThreshold = 1;
    hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
    hqspi.Init.FlashSize = 1;
    hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
    hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
    hqspi.Init.FlashID = QSPI_FLASH_ID_1;
    hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
    if(HAL_QSPI_Init(&hqspi) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
{
    static DMA_HandleTypeDef hdma_quadspi;

    GPIO_InitTypeDef GPIO_InitStruct;
    if(hqspi->Instance == QUADSPI)
    {
        /* USER CODE BEGIN QUADSPI_MspInit 0 */

        /* USER CODE END QUADSPI_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_QSPI_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();
        __HAL_RCC_QSPI_FORCE_RESET();
        __HAL_RCC_QSPI_RELEASE_RESET();

        /**QUADSPI GPIO Configuration    
    PE10     ------> QUADSPI_CLK
    PE11     ------> QUADSPI_BK1_NCS
    PE12     ------> QUADSPI_BK1_IO0
    PE13     ------> QUADSPI_BK1_IO1
    PE14     ------> QUADSPI_BK1_IO2
    PE15     ------> QUADSPI_BK1_IO3 
    */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
                              | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* QUADSPI DMA Init */
        /* QUADSPI Init */
        hdma_quadspi.Instance = DMA2_Channel7;
        hdma_quadspi.Init.Request = DMA_REQUEST_3;
        hdma_quadspi.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_quadspi.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_quadspi.Init.MemInc = DMA_MINC_ENABLE;
        hdma_quadspi.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_quadspi.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_quadspi.Init.Mode = DMA_NORMAL;
        hdma_quadspi.Init.Priority = DMA_PRIORITY_LOW;
        if(HAL_DMA_Init(&hdma_quadspi) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(hqspi, hdma, hdma_quadspi);

        /* NVIC configuration for DMA interrupt */
        HAL_NVIC_SetPriority(DMA2_Channel7_IRQn, 0x00, 0);
        HAL_NVIC_EnableIRQ(DMA2_Channel7_IRQn);

        /* QUADSPI interrupt Init */
        HAL_NVIC_SetPriority(QUADSPI_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(QUADSPI_IRQn);

        /* USER CODE BEGIN QUADSPI_MspInit 1 */

        /* USER CODE END QUADSPI_MspInit 1 */
    }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{
    if(qspiHandle->Instance == QUADSPI)
    {
        /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

        /* USER CODE END QUADSPI_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_QSPI_CLK_DISABLE();

        /**QUADSPI GPIO Configuration    
    PE10     ------> QUADSPI_CLK
    PE11     ------> QUADSPI_BK1_NCS
    PE12     ------> QUADSPI_BK1_IO0
    PE13     ------> QUADSPI_BK1_IO1
    PE14     ------> QUADSPI_BK1_IO2
    PE15     ------> QUADSPI_BK1_IO3 
    */
        HAL_GPIO_DeInit(GPIOE,
                        GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
                            | GPIO_PIN_15);

        /* QUADSPI DMA DeInit */
        HAL_DMA_DeInit(qspiHandle->hdma);

        /* QUADSPI interrupt Deinit */
        HAL_NVIC_DisableIRQ(QUADSPI_IRQn);
        /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

        /* USER CODE END QUADSPI_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
