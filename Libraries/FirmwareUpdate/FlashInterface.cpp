/**
  ******************************************************************************
  * @file    flash_if.c
  * @author  MCD Application Team
  * @brief   FLASH Interface module.
  *          This file provides set of firmware functions to manage Flash
  *          Interface functionalities.
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

/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup USER_APP_COMMON Common
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "FlashInterface.hpp"
#include "stm32l4xx_hal.h"
#include "string.h"
#include <cstdint>
#include <stdio.h>
#include "InternalFlashAddr.hpp"
#include "sfu_fwimg_regions.h"

/* Uncomment the line below if you want some debug logs */
//#define FLASH_IF_DBG
#ifdef FLASH_IF_DBG
#define FLASH_IF_TRACE printf
#else
#define FLASH_IF_TRACE(...)
#endif /* FLASH_IF_DBG */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define NB_PAGE_SECTOR_PER_ERASE 2U /*!< Nb page erased per erase */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets the page of a given address
  * @param  uAddr: Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t FlashInterface::getPage(uint32_t uAddr)
{
    uint32_t page = 0U;

    if(uAddr < (FLASH_BASE + (FLASH_BANK_SIZE)))
    {
        /* Bank 1 */
        page = (uAddr - FLASH_BASE) / FLASH_PAGE_SIZE;
    }
    else
    {
        /* Bank 2 */
        page = (uAddr - (FLASH_BASE + (FLASH_BANK_SIZE))) / FLASH_PAGE_SIZE;
    }

    return page;
}
/**
  * @brief  Gets the bank of a given address
  * @param  uAddr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
uint32_t FlashInterface::getBank(uint32_t uAddr)
{
    uint32_t bank = 0U;

    if(READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0U)
    {
        /* No Bank swap */
        if(uAddr < (FLASH_BASE + (FLASH_BANK_SIZE)))
        {
            bank = FLASH_BANK_1;
        }
        else
        {
            bank = FLASH_BANK_2;
        }
    }
    else
    {
        /* Bank swap */
        if(uAddr < (FLASH_BASE + (FLASH_BANK_SIZE)))
        {
            bank = FLASH_BANK_2;
        }
        else
        {
            bank = FLASH_BANK_1;
        }
    }

    return bank;
}

FlashInterface::FlashInterface(AbstractWatchdog& watchdog)
    : mWatchdog(watchdog)
{
}

/**
  * @brief  Gets the address of a bank
  * @param  Bank: Bank ID
  * @retval Address of the bank
  */
uint32_t FlashInterface::getBankAddr(uint32_t Bank)
{
    if(Bank == FLASH_BANK_2)
    {
        return FLASH_BASE + (FLASH_BANK_SIZE);
    }
    else
    {
        return FLASH_BASE;
    }
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval HAL Status.
  */
HAL_StatusTypeDef FlashInterface::init()
{
    HAL_StatusTypeDef ret = HAL_ERROR;

    /* Unlock the Program memory */
    if(HAL_FLASH_Unlock() == HAL_OK)
    {
        /* Clear all FLASH flags */
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
        /* Unlock the Program memory */
        if(HAL_FLASH_Lock() == HAL_OK)
        {
            ret = HAL_OK;
        }
#ifdef FLASH_IF_DBG
        else
        {
            FLASH_IF_TRACE("[FLASH_IF] Lock failure\r\n");
        }
#endif /* FLASH_IF_DBG */
    }
#ifdef FLASH_IF_DBG
    else
    {
        FLASH_IF_TRACE("[FLASH_IF] Unlock failure\r\n");
    }
#endif /* FLASH_IF_DBG */
    return ret;
}

HAL_StatusTypeDef FlashInterface::erase(FLASH_EraseInitTypeDef* pEraseInit, uint32_t* PageError)
{
    __disable_irq();
    HAL_StatusTypeDef ret = HAL_FLASHEx_Erase(pEraseInit, PageError);
    __enable_irq();

    return ret;
}

HAL_StatusTypeDef FlashInterface::program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
    __disable_irq();
    HAL_StatusTypeDef ret = HAL_FLASH_Program(TypeProgram, Address, Data);
    __enable_irq();

    return ret;
}

HAL_StatusTypeDef FlashInterface::deinit()
{
    // Nothing to do
    return HAL_OK;
}

bool FlashInterface::isAuthorizedArea(void* pStart, uint32_t uLength) const
{
    volatile uint8_t* const pEnd = reinterpret_cast<uint8_t*>(pStart) + uLength - 1;
    bool authorized = false;
#ifdef APP_GOLDEN_IMAGE
    return true;
#else
    bool startAddressInDownloadArea = (pStart >= reinterpret_cast<uint8_t*>(SlotStartAdd[SLOT_DWL_1])
                                       && pStart <= reinterpret_cast<uint8_t*>(
                                              SlotEndAdd[SLOT_DWL_1]));
    bool endAddressInDownloadArea = (pEnd >= reinterpret_cast<uint8_t*>(SlotStartAdd[SLOT_DWL_1])
                                     && pEnd <= reinterpret_cast<uint8_t*>(SlotEndAdd[SLOT_DWL_1]));

    authorized = startAddressInDownloadArea && endAddressInDownloadArea;

    if(!authorized)
    {
        bool startAddressInOtp =
            (pStart >= reinterpret_cast<uint8_t*>(internal_flash_addr::OTP_AREA_START)
             && pStart <= reinterpret_cast<uint8_t*>(internal_flash_addr::OTP_AREA_END));
        bool endAddressInOtp =
            (pEnd >= reinterpret_cast<uint8_t*>(internal_flash_addr::OTP_AREA_START)
             && pEnd <= reinterpret_cast<uint8_t*>(internal_flash_addr::OTP_AREA_END));

        authorized = startAddressInOtp && endAddressInOtp;
    }

    return authorized;
#endif
}

bool FlashInterface::write(void* pDestination, const void* pSource, uint32_t uLength)
{
    ASSERT(isAuthorizedArea(pDestination, uLength));

    return doWrite(pDestination, pSource, uLength) == HAL_OK;
}

bool FlashInterface::eraseSize(void* pStart, uint32_t uLength)
{
    ASSERT(isAuthorizedArea(pStart, uLength));

    return doEraseSize(pStart, uLength) == HAL_OK;
}

void* FlashInterface::getEnd() const
{
    // Until end of OTP area
    // This will include other regions of memory so the address must be checked
    // to make sure is it actually a flash address
    return reinterpret_cast<void*>(internal_flash_addr::OTP_AREA_END);
}

size_t FlashInterface::getPageSize() const
{
    return FLASH_PAGE_SIZE;
}

bool FlashInterface::isFlashZone(void* pStart, uint32_t uLength) const
{
    return IS_FLASH_PROGRAM_ADDRESS(reinterpret_cast<uint32_t>(pStart))
           && IS_FLASH_PROGRAM_ADDRESS(reinterpret_cast<uint32_t>(pStart) + uLength - 1U);
}

bool FlashInterface::isFlashErasableZone(void* pStart, uint32_t uLength) const
{
    return IS_FLASH_MAIN_MEM_ADDRESS(reinterpret_cast<uint32_t>(pStart))
           && IS_FLASH_MAIN_MEM_ADDRESS(reinterpret_cast<uint32_t>(pStart) + uLength - 1U);
}

void FlashInterface::refreshWatchdog()
{
    mWatchdog.refresh();
}

/**
  * @brief  This function does an erase of n (depends on Length) pages in user flash area
  * @param  pStart: Start of user flash area
  * @param  uLength: number of bytes.
  * @retval HAL status.
  */
HAL_StatusTypeDef FlashInterface::doEraseSize(void* pStart, uint32_t uLength)
{
    uint32_t page_error = 0U;
    uint32_t uStart = (uint32_t) pStart;
    FLASH_EraseInitTypeDef x_erase_init;
    HAL_StatusTypeDef e_ret_status = HAL_ERROR;
    uint32_t first_page = 0U, nb_pages = 0U;
    uint32_t chunk_nb_pages;
    uint32_t erase_command = 0U;
    uint32_t bank_number = 0U;

    /* Initialize Flash */
    e_ret_status = init();

    if(e_ret_status == HAL_OK)
    {
        /* Unlock the Flash to enable the flash control register access *************/
        if(HAL_FLASH_Unlock() == HAL_OK)
        {
            do
            {
                /* Get the 1st page to erase */
                first_page = getPage(uStart);
                bank_number = getBank(uStart);
                if(getBank(uStart + uLength - 1U) == bank_number)
                {
                    /* Get the number of pages to erase from 1st page */
                    nb_pages = getPage(uStart + uLength - 1U) - first_page + 1U;

                    /* Fill EraseInit structure*/
                    x_erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
                    x_erase_init.Banks = bank_number;

                    /* Erase flash per NB_PAGE_SECTOR_PER_ERASE to avoid watch-dog */
                    do
                    {
                        chunk_nb_pages = (nb_pages >= NB_PAGE_SECTOR_PER_ERASE)
                                             ? NB_PAGE_SECTOR_PER_ERASE
                                             : nb_pages;
                        x_erase_init.Page = first_page;
                        x_erase_init.NbPages = chunk_nb_pages;
                        first_page += chunk_nb_pages;
                        nb_pages -= chunk_nb_pages;
                        if(erase(&x_erase_init, &page_error) != HAL_OK)
                        {
                            HAL_FLASH_GetError();
                            e_ret_status = HAL_ERROR;
                        }
                        refreshWatchdog();
                    } while(nb_pages > 0);
                    erase_command = 1U;
                }
                else
                {
                    uint32_t startbank2 = getBankAddr(FLASH_BANK_2);
                    nb_pages = getPage(startbank2 - 1U) - first_page + 1U;
                    x_erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
                    x_erase_init.Banks = bank_number;
                    uLength = uLength - (startbank2 - uStart);
                    uStart = startbank2;

                    /* Erase flash per NB_PAGE_SECTOR_PER_ERASE to avoid watch-dog */
                    do
                    {
                        chunk_nb_pages = (nb_pages >= NB_PAGE_SECTOR_PER_ERASE)
                                             ? NB_PAGE_SECTOR_PER_ERASE
                                             : nb_pages;
                        x_erase_init.Page = first_page;
                        x_erase_init.NbPages = chunk_nb_pages;
                        first_page += chunk_nb_pages;
                        nb_pages -= chunk_nb_pages;
                        if(erase(&x_erase_init, &page_error) != HAL_OK)
                        {
                            HAL_FLASH_GetError();
                            e_ret_status = HAL_ERROR;
                        }
                        refreshWatchdog();
                    } while(nb_pages > 0);
                }
            } while(erase_command == 0);
            /* Lock the Flash to disable the flash control register access (recommended
      to protect the FLASH memory against possible unwanted operation) *********/
            HAL_FLASH_Lock();
        }
        else
        {
            e_ret_status = HAL_ERROR;
        }
    }

    HAL_StatusTypeDef e_deinit_status = deinit();

    if(HAL_OK == e_ret_status)
    {
        e_ret_status = e_deinit_status;
    }

    return e_ret_status;
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  pDestination: Start address for target location
  * @param  pSource: pointer on buffer with data to write
  * @param  uLength: Length of data buffer in byte. It has to be 64-bit aligned.
  * @retval HAL Status.
  */
HAL_StatusTypeDef FlashInterface::doWrite(void* pDestination, const void* pSource, uint32_t uLength)
{
    HAL_StatusTypeDef e_ret_status = HAL_ERROR;
    uint32_t i = 0U;
    const uint8_t* pdata = reinterpret_cast<const uint8_t*>(pSource);

    /* Initialize Flash */
    e_ret_status = init();

    if(e_ret_status == HAL_OK)
    {
        /* Unlock the Flash to enable the flash control register access *************/
        if(HAL_FLASH_Unlock() != HAL_OK)
        {
            return HAL_ERROR;
        }
        else
        {
            /* DataLength must be a multiple of 64 bit */
            for(i = 0U; i < uLength; i += 8U)
            {
                /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        be done by word */
                uint64_t value;
                memcpy(&value, pdata + i, sizeof(uint64_t));
                if(program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t) pDestination, value) == HAL_OK)
                {
                    /* Check the written value */
                    if(*(uint64_t*) pDestination != value)
                    {
                        /* Flash content doesn't match SRAM content */
                        e_ret_status = HAL_ERROR;
                        break;
                    }
                    /* Increment FLASH Destination address */
                    pDestination = (void*) ((uint32_t) pDestination + 8U);
                }
                else
                {
                    /* Error occurred while writing data in Flash memory */
                    e_ret_status = HAL_ERROR;
                    break;
                }
            }
            /* Lock the Flash to disable the flash control register access (recommended
      to protect the FLASH memory against possible unwanted operation) *********/
            HAL_FLASH_Lock();
        }
    }

    HAL_StatusTypeDef e_deinit_status = deinit();

    if(HAL_OK == e_ret_status)
    {
        e_ret_status = e_deinit_status;
    }

    return e_ret_status;
}
/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
