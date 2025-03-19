/*
 * AutoLock.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: Labo
 */

#include "AutoLock.h"

AutoLock::AutoLock(ILockable& lockable, DelayMs timeout)
    : m_lockable(lockable)
{
    bool bRet = m_lockable.lock(timeout);
    ASSERT(bRet);
}

AutoLock::~AutoLock()
{
    bool bRet = m_lockable.unlock();
    ASSERT(bRet);
}
