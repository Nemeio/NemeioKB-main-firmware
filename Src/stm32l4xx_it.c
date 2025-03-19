/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "cmsis_os.h"
#include "assertTools.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern QSPI_HandleTypeDef hqspi;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_spi3_rx;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    CALL_prvGetRegistersFromStackExcept(NMI_TYPE);
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    CALL_prvGetRegistersFromStackExcept(HARDFAULT_TYPE);
    /* USER CODE BEGIN HardFault_IRQn 1 */

    /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    CALL_prvGetRegistersFromStackExcept(MEMMANAGEFAULT_TYPE);
    /* USER CODE BEGIN MemoryManagement_IRQn 1 */

    /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    CALL_prvGetRegistersFromStackExcept(BUSFAULT_TYPE);
    /* USER CODE BEGIN BusFault_IRQn 1 */

    /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    CALL_prvGetRegistersFromStackExcept(USAGEFAULT_TYPE);
    /* USER CODE BEGIN UsageFault_IRQn 1 */

    /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    CALL_prvGetRegistersFromStackExcept(DEBUGMON_TYPE);
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    osSystickHandler();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/


/**
* @brief This function handles USB OTG FS global interrupt.
*/
void OTG_FS_IRQHandler(void)
{
    /* USER CODE BEGIN OTG_FS_IRQn 0 */

    /* USER CODE END OTG_FS_IRQn 0 */
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
    /* USER CODE BEGIN OTG_FS_IRQn 1 */

    /* USER CODE END OTG_FS_IRQn 1 */
}

/**
* @brief This function handles EXTI line[0] interrupts.
*/
void EXTI0_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI0_IRQn 0 */

    /* USER CODE END EXTI0_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
    /* USER CODE BEGIN EXTI0_IRQn 1 */

    /* USER CODE END EXTI0_IRQn 1 */
}
/**
* @brief This function handles EXTI line[1] interrupts.
*/
void EXTI1_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI1_IRQn 0 */

    /* USER CODE END EXTI1_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
    /* USER CODE BEGIN EXTI1_IRQn 1 */

    /* USER CODE END EXTI1_IRQn 1 */
}
/**
* @brief This function handles EXTI line[2] interrupts.
*/
void EXTI2_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI2_IRQn 0 */

    /* USER CODE END EXTI2_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
    /* USER CODE BEGIN EXTI2_IRQn 1 */

    /* USER CODE END EXTI2_IRQn 1 */
}
/**
* @brief This function handles EXTI line[3] interrupts.
*/
void EXTI3_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI3_IRQn 0 */

    /* USER CODE END EXTI3_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
    /* USER CODE BEGIN EXTI3_IRQn 1 */

    /* USER CODE END EXTI3_IRQn 1 */
}
/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI9_5_IRQn 0 */

    /* USER CODE END EXTI9_5_IRQn 0 */
    //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    /* USER CODE BEGIN EXTI9_5_IRQn 1 */
    //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    uint32_t pending = EXTI->PR1;
    if(pending & (1 << 6))
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    }
    if(pending & (1 << 9))
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
    }
    /* USER CODE END EXTI9_5_IRQn 1 */
}
/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI15_10_IRQHandler 0 */

    /* USER CODE END EXTI15_10_IRQHandler 0 */
    uint32_t pending = EXTI->PR1;
    if(pending & (1 << 13))
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    }
    if(pending & (1 << 14))
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    }
    /* USER CODE BEGIN EXTI15_10_IRQHandler 1 */


    /* USER CODE END EXTI15_10_IRQHandler 1*/
}
/**
* @brief This function handles EXTI line[4] interrupts.
*/
void EXTI4_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_IRQHandler 0 */

    /* USER CODE END EXTI4_IRQHandler 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
    /* USER CODE BEGIN EXTI4_IRQHandler 1 */


    /* USER CODE END EXTI4_IRQHandler 1*/
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
