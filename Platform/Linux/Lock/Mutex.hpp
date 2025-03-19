/*
 * Mutex.hpp
 *
 *  Created on: Oct 25, 2019
 *      Author: Labo
 */

#ifndef LOCK_MUTEX_HPP_
#define LOCK_MUTEX_HPP_

#include <mutex>
#include "AbstractNonRecursiveMutex.hpp"

class Mutex : public AbstractNonRecursiveMutex
{
public:
    Mutex();
    virtual ~Mutex() = default;

    Mutex(Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    Mutex(Mutex&&) = delete;
    Mutex const& operator=(Mutex&&) = delete;

    bool unlock();

private:
    std::timed_mutex mMutex;

    bool doLock(const DelayMs& timeout);
};

#endif /* LOCK_MUTEX_HPP_ */
