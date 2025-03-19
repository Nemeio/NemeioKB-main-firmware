/*
 * AbstractRecursiveMutex.hpp
 *
 *  Created on: Feb 21, 2020
 *      Author: Labo
 */

#ifndef LOCK_ABSTRACTRECURSIVEMUTEX_HPP_
#define LOCK_ABSTRACTRECURSIVEMUTEX_HPP_

#include "AbstractMutex.hpp"

class AbstractRecursiveMutex : public AbstractMutex
{
public:
    AbstractRecursiveMutex() = default;
    ~AbstractRecursiveMutex() override = default;
};

#endif /* LOCK_ABSTRACTRECURSIVEMUTEX_HPP_ */
