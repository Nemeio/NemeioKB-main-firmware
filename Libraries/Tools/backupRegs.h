/*
 * backupRegs.h
 *
 *  Created on: May 22, 2019
 *      Author: Labo
 */

#ifndef TOOLS_BACKUPREGS_H_
#define TOOLS_BACKUPREGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sfu_backup_registers.h"

#define BKP_REG_R0 RTC_BKP_DR0
#define BKP_REG_R1 RTC_BKP_DR1
#define BKP_REG_R2 RTC_BKP_DR2
#define BKP_REG_R3 RTC_BKP_DR3
#define BKP_REG_R4 RTC_BKP_DR4
#define BKP_REG_R5 RTC_BKP_DR5
#define BKP_REG_R6 RTC_BKP_DR6
#define BKP_REG_R7 RTC_BKP_DR7
#define BKP_REG_R8 RTC_BKP_DR8
#define BKP_REG_R9 RTC_BKP_DR9
#define BKP_REG_R10 RTC_BKP_DR10
#define BKP_REG_R11 RTC_BKP_DR11
#define BKP_REG_R12 RTC_BKP_DR12
#define BKP_REG_SP RTC_BKP_DR13
#define BKP_REG_LR RTC_BKP_DR14
#define BKP_REG_PC RTC_BKP_DR15
#define BKP_REG_XPSR RTC_BKP_DR16
#define BKP_REG_STATE RTC_BKP_DR17
#define BKP_REG_FAULTEXCEPTIONTYPE RTC_BKP_DR18

#define DEFAULT_STATE 0x50DAC001
#define ASSERTFAIL_STATE 0xBADDCAFE
#define EXCEPT_STATE 0xDEADBEEF

void writeBkpReg(uint32_t reg, uint32_t data);
uint32_t readBkpReg(uint32_t reg);

#ifdef __cplusplus
}
#endif

#endif /* TOOLS_BACKUPREGS_H_ */
