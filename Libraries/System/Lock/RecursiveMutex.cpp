/*
 * RecursiveMutex.cpp
 *
 *  Created on: Nov 5, 2019
 *      Author: Labo
 */

#include "RecursiveMutex.hpp"
#include "cmsis_os.h"
#include "FreeRTOSUtils.hpp"

RecursiveMutex::RecursiveMutex()
{
    osMutexDef(mMutexId);
    mMutexId = osRecursiveMutexCreate(osMutex(mMutexId));
    ASSERT(mMutexId != nullptr);
}

RecursiveMutex::~RecursiveMutex()
{
    osStatus ret = osMutexDelete(mMutexId);
    ASSERT(ret == osOK);
}

bool RecursiveMutex::doLock(const DelayMs& timeout)
{
    ASSERT(mMutexId != nullptr);
    return (osOK == osRecursiveMutexWait(mMutexId, delayToMsCMSIS(timeout)));
}

bool RecursiveMutex::unlock()
{
    ASSERT(mMutexId != nullptr);
    return (osOK == osRecursiveMutexRelease(mMutexId));
}
