/*
 * Mutex.cpp
 *
 *  Created on: Oct 25, 2019
 *      Author: Labo
 */

#include "Mutex.hpp"
#include "assertTools.h"
#include "cmsis_os.h"
#include "FreeRTOSUtils.hpp"

Mutex::Mutex()
{
    osMutexDef(mMutexId);
    mMutexId = osMutexCreate(osMutex(mMutexId));
    ASSERT(mMutexId != nullptr);
}

Mutex::~Mutex()
{
    osStatus ret = osMutexDelete(mMutexId);
    ASSERT(ret == osOK);
}

bool Mutex::doLock(const DelayMs& timeout)
{
    ASSERT(mMutexId != nullptr);
    return (osOK == osMutexWait(mMutexId, delayToMsCMSIS(timeout)));
}

bool Mutex::unlock()
{
    ASSERT(mMutexId != nullptr);
    return (osOK == osMutexRelease(mMutexId));
}
