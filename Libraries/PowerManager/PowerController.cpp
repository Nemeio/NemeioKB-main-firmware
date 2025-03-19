/*
 * PowerController.cpp
 *
 *  Created on: 16 juil. 2020
 *      Author: eskoric
 */

#include "PowerController.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32l4xx.h"
#include "assertTools.h"

void PowerController::sleep()
{
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void PowerController::shutdown()
{
    vTaskDelay(80); //prevent rebound
    prepareWakeUpandShutdownSTM32();
}

void PowerController::prepareWakeUpandShutdownSTM32()
{
    taskENTER_CRITICAL();

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN3_LOW);

    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4);
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN5);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF4);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF5);

    HAL_PWREx_EnterSHUTDOWNMode();

    // Shutdown has failed if we reach this line
    ASSERT(false);
}
