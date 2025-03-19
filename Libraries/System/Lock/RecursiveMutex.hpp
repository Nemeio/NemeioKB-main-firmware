/*
 * RecursiveMutex.hpp
 *
 *  Created on: Nov 5, 2019
 *      Author: Labo
 */

#ifndef LOCK_RECURSIVEMUTEX_HPP_
#define LOCK_RECURSIVEMUTEX_HPP_

#include "AbstractRecursiveMutex.hpp"
#include "cmsis_os.h"

class RecursiveMutex : public AbstractRecursiveMutex
{
public:
    RecursiveMutex();
    virtual ~RecursiveMutex();

    RecursiveMutex(RecursiveMutex&) = delete;
    RecursiveMutex& operator=(const RecursiveMutex&) = delete;
    RecursiveMutex(RecursiveMutex&&) = delete;
    RecursiveMutex const& operator=(RecursiveMutex&&) = delete;

    bool unlock();

private:
    osMutexId mMutexId;

    bool doLock(const DelayMs& timeout);
};

#endif /* LOCK_RECURSIVEMUTEX_HPP_ */
