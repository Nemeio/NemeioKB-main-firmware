/*
 * STM32Watchdog.cpp
 *
 *  Created on: Aug 20, 2019
 *      Author: Labo
 */

#include "STM32Watchdog.hpp"

STM32Watchdog::STM32Watchdog(uint32_t timeoutSeconds)
    : AbstractWatchdog(timeoutSeconds)
    , mbInitialized(false)
{
}

bool STM32Watchdog::init()
{
    return start();
}

bool STM32Watchdog::start()
{
    refreshHandle();
    mbInitialized = (HAL_OK == HAL_IWDG_Init(&mIwdgHandle));

    /* Refresh if already started */
    refresh();

    return mbInitialized;
}

void STM32Watchdog::refreshHandle()
{
    mIwdgHandle.Instance = IWDG;
    mIwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
    mIwdgHandle.Init.Reload = (mTimeoutSeconds * LSI_VALUE / 256U);
    mIwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;
}

bool STM32Watchdog::refresh()
{
    return (mbInitialized && HAL_OK == HAL_IWDG_Refresh(&mIwdgHandle));
}
