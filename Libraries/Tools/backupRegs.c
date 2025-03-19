/*
 * backupRegs.c
 *
 *  Created on: May 22, 2019
 *      Author: Labo
 */
#include "rtc.h"

void writeBkpReg(uint32_t reg, uint32_t data)
{
    HAL_RTCEx_BKUPWrite(&hrtc, reg, data);
}

uint32_t readBkpReg(uint32_t reg)
{
    return HAL_RTCEx_BKUPRead(&hrtc, reg);
}
