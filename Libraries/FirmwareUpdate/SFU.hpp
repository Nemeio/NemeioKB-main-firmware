/**
  ******************************************************************************
  * @file    sfu_app_new_image.h
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage the New
  *          Firmware Image storage and installation.
  *          This file contains the services the bootloader and the user application
  *          can use to know where to store a new FW image and request its installation.
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
#ifndef SFU_APP_NEW_IMAGE_H
#define SFU_APP_NEW_IMAGE_H

/* Includes ------------------------------------------------------------------*/
#include "ISFU.hpp"
#include "IFlashInterface.hpp"
#include "sfu_def.h"
#include "ISystem.hpp"

/** @addtogroup USER_APP User App Example
  * @{
  */

/** @addtogroup  FW_UPDATE Firmware Update Example
  * @{
  */


/** @addtogroup  SFU_APP_NEWIMG
 * @{
 */


/** @addtogroup SFU_APP_NEWIMG_Exported_Types Exported Types
  * @{
  */

/**
  * @}
  */


/** @addtogroup SFU_APP_NEWIMG_Exported_Functions
  * @{
  */

class SFU : public ISFU
{
public:
    explicit SFU(IFlashInterface& flashInterface);
    virtual ~SFU() = default;

    bool getDownloadAreaInfo(SFU_FwImageFlashTypeDef* pArea) override;
    bool validateFirmware() override;
    bool isRecoveryUpdatePerformed() override;
    void resetBackupInfo() override;
#ifdef APP_GOLDEN_IMAGE
    void recover() override;
#endif

private:
    IFlashInterface& mFlashInterface; //Used by golden image

    bool doGetDownloadAreaInfo(SFU_FwImageFlashTypeDef* pArea) const;
    bool doGetActiveAreaInfo(SFU_FwImageFlashTypeDef* pArea) const;
};

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* SFU_APP_NEW_IMAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
