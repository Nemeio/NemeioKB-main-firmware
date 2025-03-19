/*
 * Mutex.hpp
 *
 *  Created on: Oct 25, 2019
 *      Author: Labo
 */

#ifndef LOCK_MUTEX_HPP_
#define LOCK_MUTEX_HPP_

#include "AbstractNonRecursiveMutex.hpp"
#include "cmsis_os.h"

class Mutex : public AbstractNonRecursiveMutex
{
public:
    Mutex();
    virtual ~Mutex();

    Mutex(Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    Mutex(Mutex&&) = delete;
    Mutex const& operator=(Mutex&&) = delete;

    bool unlock();

private:
    osMutexId mMutexId;

    bool doLock(const DelayMs& timeout);
};

#endif /* LOCK_MUTEX_HPP_ */
