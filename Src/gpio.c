/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
#ifdef USE_ETM
    HAL_GPIO_WritePin(GPIOE,
                      /*KB_LIN_3_Pin|KB_LIN_4_Pin|KB_LIN_5_Pin|KB_LIN_6_Pin
                          |*/
                      LED_CAPSLOCK_Pin | KB_LIN_1_Pin | KB_LIN_2_Pin,
                      GPIO_PIN_RESET);
#else
    HAL_GPIO_WritePin(GPIOE,
                      KB_LIN_3_Pin | KB_LIN_4_Pin | KB_LIN_5_Pin | KB_LIN_6_Pin | LED_CAPSLOCK_Pin
                          | KB_LIN_1_Pin | KB_LIN_2_Pin,
                      GPIO_PIN_RESET);
#endif

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB,
                      PWR_PSEL_Pin | CPU_BLE__RST_Pin | LED_BT_Pin | EPD_FL_ON_Pin,
                      GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, EPD_ON_Pin | EPD_CTRL_RST__Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
                           PEPin PEPin PEPin PEPin 
                           PEPin PEPin */
#ifdef USE_ETM
    GPIO_InitStruct.Pin = /*KB_LIN_3_Pin|KB_LIN_4_Pin|KB_LIN_5_Pin|KB_LIN_6_Pin
                          |*/
        TP_CPU_PE9_Pin | KB_LIN_1_Pin | KB_LIN_2_Pin;
#else
    GPIO_InitStruct.Pin = KB_LIN_3_Pin | KB_LIN_4_Pin | KB_LIN_5_Pin | KB_LIN_6_Pin | KB_LIN_1_Pin
                          | KB_LIN_2_Pin;
#endif
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PCPin PCPin PCPin PCPin 
                           PCPin PCPin PCPin */
    GPIO_InitStruct.Pin = KB_KEY75_ALT_Pin | KB_KEY73_FN_Pin | KB_KEY74_WIN_Pin | KB_KEY57_ENTER_Pin
                          | KB_KEY58_SHIFT_LEFT_Pin | KB_KEY71_SHIFT_RIGHT_Pin
                          | KB_KEY72_CTRL_LEFT_Pin | KB_KEY78_CTRL_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PWR_STAT_Pin */
    GPIO_InitStruct.Pin = PWR_STAT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PWR_STAT_GPIO_Port, PWR_STAT_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : LED_BT_Pin */
    GPIO_InitStruct.Pin = LED_BT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, USB_HUB__RST_Pin, GPIO_PIN_SET);
    /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
    GPIO_InitStruct.Pin = USB_HUB__RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CPU_SPI3_HRDY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CPU_SPI3_HRDY_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    /* GPIO_InitStruct.Pin = BLE_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLE_NSS_GPIO_Port, &GPIO_InitStruct);*/

    /*Configure GPIO pins : PBPin PBPin */
    GPIO_InitStruct.Pin = KB_KEY77_ALT_GR_Pin | PWR__PG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin PBPin */
    GPIO_InitStruct.Pin = PWR_PSEL_Pin | CPU_BLE__RST_Pin | EPD_FL_ON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, EPD_FL_ON_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, VBUS_DET__RST_Pin, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = VBUS_DET__RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : Button_LEFT_Pin Button_RIGHT_Pin */
    GPIO_InitStruct.Pin = Button_LEFT_Pin | Button_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : LED_CAPSLOCK_Pin */
    GPIO_InitStruct.Pin = LED_CAPSLOCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_CAPSLOCK_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PDPin PDPin PDPin PDPin 
                           PDPin PDPin PDPin PDPin 
                           PDPin PDPin PDPin PDPin 
                           PDPin PDPin PDPin PDPin */
    GPIO_InitStruct.Pin = KB_COL_9_Pin | KB_COL_10_Pin | KB_COL_11_Pin | KB_COL_12_Pin
                          | KB_COL_13_Pin | KB_COL_14_Pin | KB_COL_15_Pin | KB_COL_16_Pin
                          | KB_COL_1_Pin | KB_COL_2_Pin | KB_COL_3_Pin | KB_COL_4_Pin | KB_COL_5_Pin
                          | KB_COL_6_Pin | KB_COL_7_Pin | KB_COL_8_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PCPin PCPin PCPin */
    GPIO_InitStruct.Pin = EPD_ON_Pin | EPD_CTRL_RST__Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = TP_CPU_PH3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TP_CPU_PH3_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
