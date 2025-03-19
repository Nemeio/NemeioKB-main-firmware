/**
  ******************************************************************************
  * @file    flash_if.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for FLASH Interface functionalities.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FLASH_IF_H
#define FLASH_IF_H

/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup USER_APP_COMMON Common
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
#include "../Watchdog/AbstractWatchdog.hpp"
#include "IFlashInterface.hpp"
#include "utils.hpp"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_IF_MIN_WRITE_LEN (8U) /* Flash programming by 64 bits */
/* Exported functions ------------------------------------------------------- */

class FlashInterface : public IFlashInterface
{
public:
    FlashInterface(AbstractWatchdog& watchdog);
    virtual ~FlashInterface() = default;

    bool write(void* pDestination, const void* pSource, uint32_t uLength) final;
    bool eraseSize(void* pStart, uint32_t uLength) final;

    void* getEnd() const final;
    size_t getPageSize() const final;
    bool isFlashZone(void* pStart, uint32_t uLength) const final;
    bool isFlashErasableZone(void* pStart, uint32_t uLength) const final;

private:
    AbstractWatchdog& mWatchdog;
    uint32_t getPage(uint32_t uAddr);
    uint32_t getBank(uint32_t uAddr);
    uint32_t getBankAddr(uint32_t Bank);

    HAL_StatusTypeDef init();
    HAL_StatusTypeDef deinit();
    HAL_StatusTypeDef erase(FLASH_EraseInitTypeDef* pEraseInit, uint32_t* PageError);
    HAL_StatusTypeDef program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
    HAL_StatusTypeDef doWrite(void* pDestination, const void* pSource, uint32_t uLength);
    HAL_StatusTypeDef doEraseSize(void* pStart, uint32_t uLength);
    bool isAuthorizedArea(void* pStart, uint32_t uLength) const;

    void refreshWatchdog();
};


/**
  * @}
  */

/**
  * @}
  */


#endif /* FLASH_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
