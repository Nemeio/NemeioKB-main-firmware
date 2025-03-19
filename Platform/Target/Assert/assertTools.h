/*
 * assert.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: Labo
 */

#ifndef ASSERTTOOLS_HPP_
#define ASSERTTOOLS_HPP_

#include <stdint.h>
#include <stdbool.h>
#include "armRegisters.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NMI_TYPE 0
#define HARDFAULT_TYPE 1
#define MEMMANAGEFAULT_TYPE 2
#define BUSFAULT_TYPE 3
#define USAGEFAULT_TYPE 4
#define DEBUGMON_TYPE 5

void prvGetRegistersFromStackAssertFail(uint32_t* pulStackAddress, uint32_t pc, uint32_t xpsr);
void prvGetRegistersFromStackExcept(uint32_t* pulFaultStackAddress, uint32_t exceptType);

void assertionFailed(const struct ArmRegisters* pRegisters);
void faultExceptionRaised(const struct ArmRegisters* pRegisters, uint32_t exceptType);

inline void noReturn() __attribute__((noreturn));

inline void noReturn()
{
    while(true)
    {
        /*Nothing to do */
    }
}

#define ASSERT(expr) \
    do \
    { \
        if(expr) \
        { \
        } \
        else \
        { \
            CALL_prvGetRegistersFromStackAssertFail(); \
            noReturn(); \
        } \
    } while(0)

#define CALL_prvGetRegistersFromStackAssertFail() \
    { \
        __asm volatile( \
            " stm sp,{r0-r12, lr}	                                    \n" \
            " mov r0, sp	                                            \n" \
            " mov r1, pc	                                            \n" \
            " mrs r2, xpsr	                                            \n" \
            " bl prvGetRegistersFromStackAssertFail                                              " \
            "       \n"); \
    }

#define _s_s(a) _s_(a)
#define _s_(a) #a

#define CALL_prvGetRegistersFromStackExcept(faultExceptionType) \
    { \
        __asm volatile\
	(\
		" tst lr, #4                                                \n"\
		" ite eq                                                    \n"\
		" mrseq r0, msp                                             \n"\
		" mrsne r0, psp                                             \n"\
		" mov r1, #" _s_s(faultExceptionType) "                       \n"\
		" ldr r2, =prvGetRegistersFromStackExcept                   \n"\
		" bx r2                                                     \n"\
	); \
    }

#ifdef __cplusplus
}
#endif

#endif /* ASSERTTOOLS_HPP_ */
