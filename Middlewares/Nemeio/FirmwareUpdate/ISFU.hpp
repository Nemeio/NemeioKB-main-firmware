/*
 * ISFU.hpp
 *
 *  Created on: Feb 10, 2020
 *      Author: Labo
 */

#ifndef FIRMWAREUPDATE_ISFU_HPP_
#define FIRMWAREUPDATE_ISFU_HPP_

#include <cstdint>
#include "Version.h" /* For APP_GOLDEN_IMAGE definition */

/**
  * @brief Specifies a structure containing values related to the management of multi-images in Flash.
  */
typedef struct
{
    uint32_t MaxSizeInBytes; /*!< The maximum allowed size for the FwImage in User Flash (in Bytes) */
    uint8_t* ImageAddr;      /*!< The download address for the FwImage in UserFlash */
    uint32_t ImageOffsetInBytes; /*!< Image write starts at this offset */
    uint32_t ExecutionAddr;      /*!< The execution address for the FwImage in UserFlash */
} SFU_FwImageFlashTypeDef;


class ISFU
{
public:
    ISFU() = default;
    virtual ~ISFU() = default;

    virtual bool getDownloadAreaInfo(SFU_FwImageFlashTypeDef* pArea) = 0;
    virtual bool validateFirmware() = 0;
    virtual bool isRecoveryUpdatePerformed() = 0;
    virtual void resetBackupInfo() = 0;
#ifdef APP_GOLDEN_IMAGE
    virtual void recover() = 0;
#endif
};

#endif /* FIRMWAREUPDATE_ISFU_HPP_ */
