/*
 * Mutex.cpp
 *
 *  Created on: Mar 9, 2010
 *      Author: thomas
 */

#include "RecursiveMutex.hpp"

RecursiveMutex::RecursiveMutex()
{
}

bool RecursiveMutex::doLock(const DelayMs& timeout)
{
    using Ms = std::chrono::milliseconds;
    return mMutex.try_lock_for(Ms(timeout.getMs()));
}

bool RecursiveMutex::unlock()
{
    mMutex.unlock();
    return true;
}
