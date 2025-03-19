/*
 * AbstractHWTimer.hpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_ABSTRACTHWTIMER_HPP_
#define NEMEIO_SYSTEM_TIMER_ABSTRACTHWTIMER_HPP_

#include "AbstractTimer.hpp"

class AbstractHWTimer : public AbstractTimer
{
public:
    AbstractHWTimer() = default;
    virtual ~AbstractHWTimer() = default;

    virtual RetCode setDelayUs(const DelayUs& delay) = 0;
};

#endif /* NEMEIO_SYSTEM_TIMER_ABSTRACTHWTIMER_HPP_ */
