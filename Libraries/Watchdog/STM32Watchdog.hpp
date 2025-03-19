/*
 * STM32Watchdog.hpp
 *
 *  Created on: Aug 20, 2019
 *      Author: Labo
 */

#ifndef WATCHDOG_STM32WATCHDOG_HPP_
#define WATCHDOG_STM32WATCHDOG_HPP_

#include "stm32l4xx_hal.h"
#include "AbstractWatchdog.hpp"

class STM32Watchdog : public AbstractWatchdog
{
public:
    STM32Watchdog(uint32_t timeoutSeconds);
    virtual ~STM32Watchdog() = default;

    bool init() final;
    bool start() final;
    bool refresh() final;

private:
    bool mbInitialized;
    IWDG_HandleTypeDef mIwdgHandle;

    void refreshHandle();
};

#endif /* WATCHDOG_STM32WATCHDOG_HPP_ */
