/*
 * AbstractNonRecursiveMutex.hpp
 *
 *  Created on: Feb 21, 2020
 *      Author: Labo
 */

#ifndef LOCK_ABSTRACTNONRECURSIVEMUTEX_HPP_
#define LOCK_ABSTRACTNONRECURSIVEMUTEX_HPP_

#include "AbstractMutex.hpp"

class AbstractNonRecursiveMutex : public AbstractMutex
{
public:
    AbstractNonRecursiveMutex() = default;
    ~AbstractNonRecursiveMutex() override = default;
};

#endif /* LOCK_ABSTRACTNONRECURSIVEMUTEX_HPP_ */
