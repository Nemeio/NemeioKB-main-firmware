/**
  ******************************************************************************
  * @file    sfu_app_new_image.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage the New Firmware
  *          Image storage and installation.
  *          This file contains the services the user application can use to
  *          know where to store a new FW image and request its installation.
  *          The same services are offered to the local loader thanks to a similar
  *          file integrated in SB_SFU.
  * @note    This file is compiled in the scope of the User Application.
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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#define SFU_FWIMG_COMMON_C

#include "sfu_def.h"
#include "sfu_fwimg_regions.h"
#include "se_def_metadata.h"
#include "se_interface_application.h"
#include "backupRegs.h"
#include <string.h> /* needed for memset (see WriteInstallHeader)*/
#include "SFU.hpp"

/**
  * @brief  Provide the area descriptor to write a FW image in Flash.
  *         This function is used by the User Application to know where to store a new Firmware Image before asking for its installation.
  * @param  pArea pointer to area descriptor
  * @retval HAL_OK if successful, otherwise HAL_ERROR
  */
bool SFU::doGetDownloadAreaInfo(SFU_FwImageFlashTypeDef* pArea) const
{
    bool bRet = false;
    if(pArea != nullptr)
    {
        pArea->ImageAddr = reinterpret_cast<uint8_t*>(SlotStartAdd[SLOT_DWL_1]);
        pArea->MaxSizeInBytes = SLOT_SIZE(SLOT_DWL_1);
        pArea->ImageOffsetInBytes = SFU_IMG_IMAGE_OFFSET;
        bRet = true;
    }
    return bRet;
}

bool SFU::doGetActiveAreaInfo(SFU_FwImageFlashTypeDef* pArea) const
{
    bool bRet = false;
    if(pArea != nullptr)
    {
        pArea->ImageAddr = reinterpret_cast<uint8_t*>(SlotStartAdd[SLOT_ACTIVE_1]);
        pArea->MaxSizeInBytes = SLOT_SIZE(SLOT_ACTIVE_1);
        pArea->ImageOffsetInBytes = SFU_IMG_IMAGE_OFFSET;
        bRet = true;
    }
    return bRet;
}

SFU::SFU(IFlashInterface& flashInterface)
    : mFlashInterface(flashInterface)
{
}

bool SFU::getDownloadAreaInfo(SFU_FwImageFlashTypeDef* pArea)
{
    return doGetDownloadAreaInfo(pArea);
}

bool SFU::validateFirmware()
{
    bool ret = false;
    uint32_t slot_number = SLOT_ACTIVE_1; // There is only one active firmware image
    SE_ErrorStatus se_retCode = SE_ERROR;
    SE_StatusTypeDef se_Status = SE_KO;
    SE_FwStateTypeDef fw_state = FWIMG_STATE_INVALID;

    SE_APP_GetActiveFwState(&se_Status, slot_number, &fw_state);
    switch(fw_state)
    {
    case FWIMG_STATE_SELFTEST:
        se_retCode = SE_APP_ValidateFw(&se_Status, slot_number);
        if((SE_SUCCESS == se_retCode) && (SE_OK == se_Status))
        {
            ret = true;
        }
        break;
    case FWIMG_STATE_VALID:
    case FWIMG_STATE_VALID_ALL:
        ret = true;
        break;
    default:
        break;
    }

    return ret;
}

bool SFU::isRecoveryUpdatePerformed()
{
    return (readBkpReg(BKP_REG_SFU) & BKP_REG_SFU_UPDATE_FROM_LOADER_MASK) != 0;
}

void SFU::resetBackupInfo()
{
    writeBkpReg(BKP_REG_SFU, 0);
}

#ifdef APP_GOLDEN_IMAGE
void SFU::recover()
{
    SFU_FwImageFlashTypeDef fwImageActiveArea;

    /* In case of powerfailure we validate this version to make sure it recover at next boot */
    validateFirmware();

    if(doGetActiveAreaInfo(&fwImageActiveArea))
    {
        /* Erase will enable interrupt no matter what. So, reset will by caused by watchdog because
         * interrupt vector will be erased and thus invalid.
         */
        mFlashInterface.eraseSize(fwImageActiveArea.ImageAddr, mFlashInterface.getPageSize());
    }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
