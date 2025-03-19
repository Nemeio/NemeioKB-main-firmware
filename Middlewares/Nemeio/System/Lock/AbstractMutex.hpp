/*
 * AbstractMutex.hpp
 *
 *  Created on: Nov 5, 2019
 *      Author: Labo
 */

#ifndef LOCK_ABSTRACTMUTEX_HPP_
#define LOCK_ABSTRACTMUTEX_HPP_

#include "SystemTypes.hpp"
#include "utils.hpp"
#include "ILockable.hpp"

class AbstractMutex : public ILockable
{
public:
    AbstractMutex() = default;
    ~AbstractMutex() override = default;

    bool lock(DelayMs timeout = DelayMs(DelayMs::INFINITE_DELAY)) final;

private:
    virtual bool doLock(const DelayMs& timeout) = 0;
};

#endif /* LOCK_ABSTRACTMUTEX_HPP_ */
