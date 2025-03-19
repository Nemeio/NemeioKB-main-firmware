/*
 * timing_macros.h
 *
 *  Created on: Jun 13, 2018
 *      Author: Labo
 */

#ifndef TIMING_MACROS_H_
#define TIMING_MACROS_H_
#include <stdio.h>

#define TRACE_FUNC_ENTRY() //printf("Time;%d;enter %s\n",HAL_GetTick(), __PRETTY_FUNCTION__)
#define TRACE_FUNC_EXIT()  //printf("Time;%d;exit %s\n",HAL_GetTick() ,__PRETTY_FUNCTION__)
#define TRACE_FUNC_ACTION( \
    ACTION, \
    ...) //printf("Time;%d;in %s, action: " ACTION "\n", HAL_GetTick(), __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define TRACE_DBG(...)              //printf(__VA_ARGS__)
#define TRACE_FUNC_DBG(ACTION, ...) //printf("%s " ACTION "\n", __PRETTY_FUNCTION__, ##__VA_ARGS__)

#endif /* TIMING_MACROS_H_ */
