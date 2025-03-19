/*
 * AutoLock.h
 *
 *  Created on: Mar 14, 2017
 *      Author: Labo
 */

#ifndef LOCK_AUTOLOCK_H_
#define LOCK_AUTOLOCK_H_

#include "SystemTypes.hpp"
#include "ILockable.hpp"

class AutoLock
{
public:
    AutoLock(ILockable& lockable, DelayMs timeout = DelayMs(DEFAULT_DELAY_MS));
    virtual ~AutoLock();

    AutoLock(AutoLock&) = delete;
    AutoLock& operator=(const AutoLock&) = delete;
    AutoLock(AutoLock&&) = delete;
    AutoLock const& operator=(AutoLock&&) = delete;

private:
    static constexpr int32_t DEFAULT_DELAY_MS = 5000;
    ILockable& m_lockable;
};

#endif /* LOCK_AUTOLOCK_H_ */
