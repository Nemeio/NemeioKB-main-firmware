/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <cstring>

#include "FreeRTOSConfig.h"
#include "UsbHID.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "quadspi.h"
#include "dma.h"
#include "usb_device.h"
#include "usbd_custom_composite.h"
#include "tim.h"
#include "gpio.h"
#include "rtc.h"
#include "spi.h"
#include "i2c.h"
#include "FreeRTOSConfig.h"

/* USER CODE BEGIN Includes */

#include "diag/Trace.h"
#include "Application.hpp"
#include "SystemRuntime.hpp"
#include "SEGGER_RTT.h"
#include "task.h"
#include "TraceLogger.hpp"

extern "C" int _kill(int pid, int sig);

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define MODULE "main"

static osThreadId defaultTaskHandle;

static constexpr osPriority MAIN_TASK_PRIORITY = osPriorityHigh;
static constexpr configSTACK_DEPTH_TYPE MAIN_TASK_STACK_SIZE = 1400;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
extern "C" void MX_FREERTOS_Init(void);
extern "C" void initialise_monitor_handles(void);
extern "C" void SystemClock_Config(void);
extern "C" void Error_Handler(void);

void StartDefaultTask(void const* argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
extern "C" int io_putchar(int ch)
{
#ifndef USE_ETM
//ITM_SendChar(ch);
#endif
    return 0;
}

#ifndef RTT_LOG
extern "C" int _write(int file, char* ptr, int len)
{
#ifndef USE_ETM
//	for (int DataIdx = 0; DataIdx < len; DataIdx++)
//	{
//	   ITM_SendChar( *ptr++ );
//	}
#endif
    return len;
}
#endif //RTT_LOG

#if __cpp_exceptions
#error "Exception-less implementation for new/delete"
#endif

void* operator new(std::size_t sz)
{
    void* p = pvPortMalloc(sz);
    if(!p)
    {
        ASSERT(false);
    }
    return p;
}

void* operator new[](std::size_t sz)
{
    void* p = pvPortMalloc(sz);
    if(!p)
    {
        ASSERT(false);
    }
    return p;
}

void* operator new(std::size_t sz, const std::nothrow_t&) noexcept
{
    return pvPortMalloc(sz);
}

void* operator new[](std::size_t sz, const std::nothrow_t&) noexcept
{
    return pvPortMalloc(sz);
}

void operator delete(void* ptr)
{
    return vPortFree(ptr);
}

void operator delete[](void* ptr)
{
    return vPortFree(ptr);
}

extern "C" void* malloc(size_t size)
{
    return pvPortMalloc(size);
}

extern "C" void free(void* ptr)
{
    vPortFree(ptr);
}

extern "C" void* calloc(size_t nitems, size_t size)
{
    void* mem = malloc(nitems * size);
    if(mem)
    {
        memset(mem, 0, nitems * size);
    }
    return mem;
}

static StaticTask_t xMainTaskTCBBuffer;

static StackType_t xMainStack[MAIN_TASK_STACK_SIZE];
/* USER CODE END 0 */

extern "C" int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_RTC_Init();

    /* USER CODE BEGIN 2 */
#ifdef RTT_LOG
    SEGGER_RTT_Init();
#endif
    /* USER CODE END 2 */

    /* Call init function for freertos objects (in freertos.c) */
    osThreadStaticDef(defaultTask,
                      StartDefaultTask,
                      MAIN_TASK_PRIORITY,
                      0,
                      MAIN_TASK_STACK_SIZE,
                      xMainStack,
                      &xMainTaskTCBBuffer);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    if(defaultTaskHandle == NULL)
        trace_printf("Error creating defaultTask\n");

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    RCC_CRSInitTypeDef RCC_CRSInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSI
                                       | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
                                  | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_USB;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the main internal regulator output voltage 
    */
    if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /**Enable the SYSCFG APB clock 
    */
    __HAL_RCC_CRS_CLK_ENABLE();

    /**Configures CRS 
    */
    RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;
    RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;
    RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
    RCC_CRSInitStruct.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
    RCC_CRSInitStruct.ErrorLimitValue = 34;
    RCC_CRSInitStruct.HSI48CalibrationValue = 32;

    HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}


/* USER CODE BEGIN 4 */

void StartDefaultTask(void const* argument)
{
    static SystemRuntime runtime;
    LOG_INFO(MODULE, "Start");
    runtime.start();

    vTaskDelete(NULL);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t, char*)
{
    ASSERT(false);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
extern "C" void _Error_Handler(char* file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    ASSERT(false);

    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
