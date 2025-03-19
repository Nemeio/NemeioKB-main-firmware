/*
 * AbstractTimer.hpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_SYSTEM_TIMER_ABSTRACTTIMER_HPP_
#define NEMEIO_SYSTEM_TIMER_ABSTRACTTIMER_HPP_

#include "ITimer.hpp"
#include "SystemTypes.hpp"

class AbstractTimer : public ITimer
{
public:
    AbstractTimer();
    virtual ~AbstractTimer() = default;

    void registerSingleListener(ITimerListener& listener);

protected:
    ITimerListener* mpTimerListener;
};

#endif /* NEMEIO_SYSTEM_TIMER_ABSTRACTTIMER_HPP_ */
