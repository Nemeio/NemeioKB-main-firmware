#include "assertTools.h"
#include "stm32l496xx.h"
#include "systemUtils.h"
#include "backupRegs.h"

void prvGetRegistersFromStackExcept(uint32_t* pulFaultStackAddress, uint32_t exceptType)
{
    struct ArmRegisters registers;

    registers.R0 = pulFaultStackAddress[0];
    registers.R1 = pulFaultStackAddress[1];
    registers.R2 = pulFaultStackAddress[2];
    registers.R3 = pulFaultStackAddress[3];

    asm("mov %0,r4" : "=r"(registers.R4));
    asm("mov %0,r5" : "=r"(registers.R5));
    asm("mov %0,r6" : "=r"(registers.R6));
    asm("mov %0,r7" : "=r"(registers.R7));
    asm("mov %0,r8" : "=r"(registers.R8));
    asm("mov %0,r9" : "=r"(registers.R9));
    asm("mov %0,r10" : "=r"(registers.R10));
    asm("mov %0,r11" : "=r"(registers.R11));

    registers.R12 = pulFaultStackAddress[4];
    registers.SP = (uint32_t) pulFaultStackAddress;
    registers.LR = pulFaultStackAddress[5];
    registers.PC = pulFaultStackAddress[6];
    registers.xPSR = pulFaultStackAddress[7];

    faultExceptionRaised(&registers, exceptType);
}

void prvGetRegistersFromStackAssertFail(uint32_t* pulStackAddress, uint32_t pc, uint32_t xpsr)
{
    struct ArmRegisters registers;

    registers.R0 = pulStackAddress[0];
    registers.R1 = pulStackAddress[1];
    registers.R2 = pulStackAddress[2];
    registers.R3 = pulStackAddress[3];
    registers.R4 = pulStackAddress[4];
    registers.R5 = pulStackAddress[5];
    registers.R6 = pulStackAddress[6];
    registers.R7 = pulStackAddress[7];
    registers.R8 = pulStackAddress[8];
    registers.R9 = pulStackAddress[9];
    registers.R10 = pulStackAddress[10];
    registers.R11 = pulStackAddress[11];
    registers.R12 = pulStackAddress[12];
    registers.SP = (uint32_t) pulStackAddress;
    registers.LR = pulStackAddress[13];
    registers.PC = pc;
    registers.xPSR = xpsr;

    assertionFailed(&registers);
}

void systemFailed(uint32_t state, const struct ArmRegisters* pRegisters)
{
    writeBkpReg(BKP_REG_STATE, state);

    if(pRegisters != NULL)
    {
        writeBkpReg(BKP_REG_R0, pRegisters->R0);
        writeBkpReg(BKP_REG_R1, pRegisters->R1);
        writeBkpReg(BKP_REG_R2, pRegisters->R2);
        writeBkpReg(BKP_REG_R3, pRegisters->R3);
        writeBkpReg(BKP_REG_R4, pRegisters->R4);
        writeBkpReg(BKP_REG_R5, pRegisters->R5);
        writeBkpReg(BKP_REG_R6, pRegisters->R6);
        writeBkpReg(BKP_REG_R7, pRegisters->R7);
        writeBkpReg(BKP_REG_R8, pRegisters->R8);
        writeBkpReg(BKP_REG_R9, pRegisters->R9);
        writeBkpReg(BKP_REG_R10, pRegisters->R10);
        writeBkpReg(BKP_REG_R11, pRegisters->R11);
        writeBkpReg(BKP_REG_R12, pRegisters->R12);
        writeBkpReg(BKP_REG_SP, pRegisters->SP);
        writeBkpReg(BKP_REG_LR, pRegisters->LR);
        writeBkpReg(BKP_REG_PC, pRegisters->PC);
        writeBkpReg(BKP_REG_XPSR, pRegisters->xPSR);
    }

    SystemReset();
}

void assertionFailed(const struct ArmRegisters* pRegisters)
{
    systemFailed(ASSERTFAIL_STATE, pRegisters);
}

void faultExceptionRaised(const struct ArmRegisters* pRegisters, uint32_t exceptType)
{
    writeBkpReg(BKP_REG_FAULTEXCEPTIONTYPE, exceptType);

    systemFailed(EXCEPT_STATE, pRegisters);
}
