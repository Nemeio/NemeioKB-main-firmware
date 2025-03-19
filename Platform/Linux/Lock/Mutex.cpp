/*
 * Mutex.cpp
 *
 *  Created on: Mar 9, 2010
 *      Author: thomas
 */

#include <iostream>
#include "Mutex.hpp"

Mutex::Mutex()
{
}

bool Mutex::doLock(const DelayMs& timeout)
{
    using Ms = std::chrono::milliseconds;
    return mMutex.try_lock_for(Ms(timeout.getMs()));
}

bool Mutex::unlock()
{
    mMutex.unlock();
    return true;
}
