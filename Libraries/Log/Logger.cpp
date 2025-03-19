/*
 * Logger.cpp
 *
 *  Created on: May 22, 2019
 *      Author: Labo
 */

#include "Logger.hpp"
#include "backupRegs.h"
#include "assertTools.h"
#include "AssertFailEvent.hpp"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_rtc_ex.h"
#include "FaultExceptionEvent.hpp"
#include "TestEvent.hpp"
#include "ApplicationConfig.h"


Logger::Logger(AbstractLogAdapter& eventLogAdapter)
    : mEventLogAdapter(eventLogAdapter)
{
}

void Logger::logTestEvent(etl::string_view description)
{
    addTestEvent(description);
    mEventLogAdapter.flush();
}

void Logger::addTestEvent(etl::string_view description)
{
    TestEvent event(description);
    mEventLogAdapter << event;
}

void Logger::addAssertionFailedEvent(const ArmRegisters& registers)
{
    AssertFailEvent evt(registers);
    mEventLogAdapter << evt;
}

void Logger::addFaultExceptionRaisedEvent(const ArmRegisters& registers, uint32_t exceptionType)
{
    FaultExceptionEvent evt(registers, exceptionType);
    mEventLogAdapter << evt;
}

void Logger::checkForSysFailEvents()
{
    uint32_t errorState = readBkpReg(BKP_REG_STATE);

    if(errorState != DEFAULT_STATE)
    {
        ArmRegisters registers;

        registers.R0 = readBkpReg(BKP_REG_R0);
        registers.R1 = readBkpReg(BKP_REG_R1);
        registers.R2 = readBkpReg(BKP_REG_R2);
        registers.R3 = readBkpReg(BKP_REG_R3);
        registers.R4 = readBkpReg(BKP_REG_R4);
        registers.R5 = readBkpReg(BKP_REG_R5);
        registers.R6 = readBkpReg(BKP_REG_R6);
        registers.R7 = readBkpReg(BKP_REG_R7);
        registers.R8 = readBkpReg(BKP_REG_R8);
        registers.R9 = readBkpReg(BKP_REG_R9);
        registers.R10 = readBkpReg(BKP_REG_R10);
        registers.R11 = readBkpReg(BKP_REG_R11);
        registers.R12 = readBkpReg(BKP_REG_R12);
        registers.SP = readBkpReg(BKP_REG_SP);
        registers.LR = readBkpReg(BKP_REG_LR);
        registers.PC = readBkpReg(BKP_REG_PC);
        registers.xPSR = readBkpReg(BKP_REG_XPSR);

        if(ASSERTFAIL_STATE == errorState)
        {
            addAssertionFailedEvent(registers);
        }
        else if(EXCEPT_STATE == errorState)
        {
            uint32_t exceptionType = readBkpReg(BKP_REG_FAULTEXCEPTIONTYPE);
            addFaultExceptionRaisedEvent(registers, exceptionType);
        }

        mEventLogAdapter.flush();
    }


    writeBkpReg(BKP_REG_STATE, DEFAULT_STATE);
}