/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* Keyboard columns used for scanning user input */
#define KB_LIN_1_Pin GPIO_PIN_0
#define KB_LIN_1_GPIO_Port GPIOE
#define KB_LIN_2_Pin GPIO_PIN_1
#define KB_LIN_2_GPIO_Port GPIOE
#define KB_LIN_3_Pin GPIO_PIN_2
#define KB_LIN_3_GPIO_Port GPIOE
#define KB_LIN_4_Pin GPIO_PIN_3
#define KB_LIN_4_GPIO_Port GPIOE
#define KB_LIN_5_Pin GPIO_PIN_4
#define KB_LIN_5_GPIO_Port GPIOE
#define KB_LIN_6_Pin GPIO_PIN_5
#define KB_LIN_6_GPIO_Port GPIOE

/* Keyboard lines used for scanning user input */
#define KB_COL_1_Pin GPIO_PIN_0
#define KB_COL_1_GPIO_Port GPIOD
#define KB_COL_2_Pin GPIO_PIN_1
#define KB_COL_2_GPIO_Port GPIOD
#define KB_COL_3_Pin GPIO_PIN_2
#define KB_COL_3_GPIO_Port GPIOD
#define KB_COL_4_Pin GPIO_PIN_3
#define KB_COL_4_GPIO_Port GPIOD
#define KB_COL_5_Pin GPIO_PIN_4
#define KB_COL_5_GPIO_Port GPIOD
#define KB_COL_6_Pin GPIO_PIN_5
#define KB_COL_6_GPIO_Port GPIOD
#define KB_COL_7_Pin GPIO_PIN_6
#define KB_COL_7_GPIO_Port GPIOD
#define KB_COL_8_Pin GPIO_PIN_7
#define KB_COL_8_GPIO_Port GPIOD
#define KB_COL_9_Pin GPIO_PIN_8
#define KB_COL_9_GPIO_Port GPIOD
#define KB_COL_10_Pin GPIO_PIN_9
#define KB_COL_10_GPIO_Port GPIOD
#define KB_COL_11_Pin GPIO_PIN_10
#define KB_COL_11_GPIO_Port GPIOD
#define KB_COL_12_Pin GPIO_PIN_11
#define KB_COL_12_GPIO_Port GPIOD
#define KB_COL_13_Pin GPIO_PIN_12
#define KB_COL_13_GPIO_Port GPIOD
#define KB_COL_14_Pin GPIO_PIN_13
#define KB_COL_14_GPIO_Port GPIOD
#define KB_COL_15_Pin GPIO_PIN_14
#define KB_COL_15_GPIO_Port GPIOD
#define KB_COL_16_Pin GPIO_PIN_15
#define KB_COL_16_GPIO_Port GPIOD

/* Keyboard modifiers pin configuration */
#define KB_KEY75_ALT_Pin GPIO_PIN_0
#define KB_KEY75_ALT_GPIO_Port GPIOC
#define KB_KEY73_FN_Pin GPIO_PIN_1
#define KB_KEY73_FN_GPIO_Port GPIOC
#define KB_KEY74_WIN_Pin GPIO_PIN_2
#define KB_KEY74_WIN_GPIO_Port GPIOC
#define KB_KEY57_ENTER_Pin GPIO_PIN_3
#define KB_KEY57_ENTER_GPIO_Port GPIOC
#define KB_KEY58_SHIFT_LEFT_Pin GPIO_PIN_4
#define KB_KEY58_SHIFT_LEFT_GPIO_Port GPIOC
#define KB_KEY71_SHIFT_RIGHT_Pin GPIO_PIN_5
#define KB_KEY71_SHIFT_RIGHT_GPIO_Port GPIOC
#define KB_KEY72_CTRL_LEFT_Pin GPIO_PIN_6
#define KB_KEY72_CTRL_LEFT_GPIO_Port GPIOC
#define KB_KEY78_CTRL_RIGHT_Pin GPIO_PIN_7
#define KB_KEY78_CTRL_RIGHT_GPIO_Port GPIOC
#define KB_KEY77_ALT_GR_Pin GPIO_PIN_10
#define KB_KEY77_ALT_GR_GPIO_Port GPIOB

/*
 * SPI 1 Pins configuration :
 * The SPI 1 interface is unused and not connected
 */
#define SPI1_NSS_Pin GPIO_PIN_0
#define SPI1_NSS_GPIO_Port GPIOB
#define SPI1_SCLK_Pin GPIO_PIN_5
#define SPI1_SCLK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_PORT GPIOA

/* TODO: What are those pins for ? */
#define CPU_FL_PWM_CTRL_Pin GPIO_PIN_0
#define CPU_FL_PWM_CTRL_GPIO_Port GPIOA
#define Button_ON_OFF_Pin GPIO_PIN_6
#define Button_ON_OFF_GPIO_Port GPIOE
#define Button_LEFT_Pin GPIO_PIN_7
#define Button_LEFT_GPIO_Port GPIOE
#define Button_RIGHT_Pin GPIO_PIN_8
#define Button_RIGHT_GPIO_Port GPIOE
#define LED_CAPSLOCK_Pin GPIO_PIN_9
#define LED_CAPSLOCK_GPIO_Port GPIOE
#define TP_CPU_PC7_Pin GPIO_PIN_7
#define TP_CPU_PC7_GPIO_Port GPIOC
#define TP_CPU_PH3_Pin GPIO_PIN_3
#define TP_CPU_PH3_GPIO_Port GPIOH
#define LED_BT_Pin GPIO_PIN_8
#define LED_BT_GPIO_Port GPIOB

#ifdef USE_ETM
#define TP_CPU_PE9_Pin GPIO_PIN_9
#endif

#define CPU_WAKE_UP_Pin GPIO_PIN_13
#define CPU_WAKE_UP_GPIO_Port GPIOC

/* Power Pins configuration */
#define PWR__PG_Pin GPIO_PIN_4
#define PWR__PG_GPIO_Port GPIOB
#define PWR_STAT_Pin GPIO_PIN_1
#define PWR_STAT_GPIO_Port GPIOB
#define PWR_PSEL_Pin GPIO_PIN_2
#define PWR_PSEL_GPIO_Port GPIOB

#define VBUS_DET__RST_Pin GPIO_PIN_5
#define VBUS_DET__RST_GPIO_Port GPIOB
#define VBUS_DET_Pin GPIO_PIN_9
#define VBUS_DET_GPIO_Port GPIOA
/*
 * BLE Pins configuration :
 * Those pins are used to communicate with the NRF SOC
 * in order to control radio communication ie : BLE
 */
#define CPU_BLE__RST_Pin GPIO_PIN_11
#define CPU_BLE__RST_GPIO_Port GPIOB
#define BLE_NSS_Pin GPIO_PIN_12
#define BLE_NSS_GPIO_Port GPIOB
#define BLE_MISO_Pin GPIO_PIN_14
#define BLE_MISO_GPIO_Port GPIOB
#define BLE_MOSI_Pin GPIO_PIN_15
#define BLE_MOSI_GPIO_Port GPIOB
#define BLE_HRDY_Pin GPIO_PIN_1
#define BLE_HRDY_GPIO_Port GPIOA
#define BLE_DTS_Pin GPIO_PIN_3
#define BLE_DTS_GPIO_Port GPIOA

/*
 * EDP Pins configuration :
 * Those pins are used to control the e-ink screen.
 * EDP_FL_ON controls the Front light status
 */
#define EPD_ON_Pin GPIO_PIN_8
#define EPD_ON_GPIO_Port GPIOC
#define EPD_CTRL_RST__Pin GPIO_PIN_9
#define EPD_CTRL_RST__GPIO_Port GPIOC
#define EPD_FL_ON_Pin GPIO_PIN_9
#define EPD_FL_ON_GPIO_Port GPIOB
#define CPU_SPI3_HRDY_Pin GPIO_PIN_2
#define CPU_SPI3_HRDY_GPIO_Port GPIOA
#define EPD_BUSYN_Pin CPU_SPI3_HRDY_Pin
#define EPD_BUSYN_GPIO_Port CPU_SPI3_HRDY_GPIO_Port
#define CPU_SPI3_NSS_Pin GPIO_PIN_4
#define CPU_SPI3_NSS_GPIO_Port GPIOA
#define EPD_CSN_Pin CPU_SPI3_NSS_Pin
#define EPD_CSN_GPIO_Port CPU_SPI3_NSS_GPIO_Port
#define EPD_SCLK_Pin GPIO_PIN_10
#define EPD_SCLK_GPIO_Port GPIOC
#define EPD_MISO_Pin GPIO_PIN_11
#define EPD_MISO_GPIO_Port GPIOC
#define EPD_MOSI_Pin GPIO_PIN_12
#define EPD_MOSI_GPIO_Port GPIOC

/*
 * QSPI Pins configuration :
 * Using 2 unidirectional data pins in SINGLE SPI mode
 * Using 2 bidirectional data pins in DUAL SPI mode
 * Using 4 bidirectional data pins in QUAD SPI mode
 */
#define SF_QSPI_SCK_Pin GPIO_PIN_10
#define SF_QSPI_SCK_GPIO_Port GPIOE
#define SF_QSPI_CS_Pin GPIO_PIN_11
#define SF_QSPI_CS_GPIO_Port GPIOE
#define SF_QSPI_IO0_Pin GPIO_PIN_12
#define SF_QSPI_IO0_GPIO_Port GPIOE
#define SF_QSPI_IO1_Pin GPIO_PIN_13
#define SF_QSPI_IO1_GPIO_Port GPIOE
#define SF_QSPI_IO2_Pin GPIO_PIN_14
#define SF_QSPI_IO2_GPIO_Port GPIOE
#define SF_QSPI_IO3_Pin GPIO_PIN_15
#define SF_QSPI_IO3_GPIO_Port GPIOE

/* Mapping for QSPI WP and QSPI HOLD if usage is required */
#define SF_QSPI_WP_Pin SF_QSPI_IO2_Pin
#define SF_QSPI_WP_GPIO_Port SF_QSPI_IO2_GPIO_Port
#define SF_QSPI_HOLD_Pin SF_QSPI_IO3_Pin
#define SF_QSPI_HOLD_GPIO_Port SF_QSPI_IO3_GPIO_Port

/* SPI CONFIGURATION IN SINGLE SPI MODE */
#define SF_SINGLE_MOSI_Pin SF_QSPI_IO0_Pin
#define SF_SINGLE_MOSI_GPIO_Port SF_QSPI_IO0_GPIO_Port
#define SF_SINGLE_MISO_Pin SF_QSPI_IO1_Pin
#define SF_SINGLE_MISO_GPIO_Port SF_QSPI_i01_GPIO_Port

/*
 * In DUAL SPI MODE IO0 and IO1 are both used for data
 * In this mode all the instruction, address, alternate-byte and data phases have to be
 * configured in dual-SPI mode by setting the IMODE/ADMODE/ABMODE/DMODE fields in
 * QUADSPI_CCR to 10
 */

/* Vibration Pin configuration */
#define CPU_VIB_CTRL_Pin GPIO_PIN_8
#define CPU_VIB_CTRL_GPIO_Port GPIOA

/* Usb HUB pins configuration */
#define USB_HUB__RST_Pin GPIO_PIN_10
#define USB_HUB__RST_GPIO_Port GPIOA

#define SCREEN_SPI_INSTANCE SPI3
#define BLE_SPI_INSTANCE SPI2
#define SERIALFLASH_QSPI_INSTANCE QUADSPI
#define FRONTLIGHT_PWM_TIM_INSTANCE TIM2
#define FRONTLIGHT_PWM_TIM_CHANNEL TIM_CHANNEL_1
#define KEYPAD_SCAN_TIMER_TIM_INSTANCE TIM3

#define IS31FL3236A_I2C_INSTANCE I2C1


/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
extern "C" {
#endif
void _Error_Handler(char*, int);

// #define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
*/

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
