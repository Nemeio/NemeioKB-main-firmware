/*
 * Mutex.hpp
 *
 *  Created on: Oct 25, 2019
 *      Author: Labo
 */

#ifndef LOCK_RECURSIVE_MUTEX_HPP_
#define LOCK_RECURSIVE_MUTEX_HPP_

#include <mutex>
#include "AbstractNonRecursiveMutex.hpp"

class RecursiveMutex : public AbstractNonRecursiveMutex
{
public:
    RecursiveMutex();
    virtual ~RecursiveMutex() = default;

    RecursiveMutex(RecursiveMutex&) = delete;
    RecursiveMutex& operator=(const RecursiveMutex&) = delete;
    RecursiveMutex(RecursiveMutex&&) = delete;
    RecursiveMutex const& operator=(RecursiveMutex&&) = delete;

    bool unlock();

private:
    std::recursive_timed_mutex mMutex;

    bool doLock(const DelayMs& timeout);
};

#endif /* LOCK_RECURSIVE_MUTEX_HPP_ */
