/*
 * AbstractTimer.cpp
 *
 *  Created on: 25 juin 2020
 *      Author: eskoric
 */

#include "AbstractTimer.hpp"

AbstractTimer::AbstractTimer()
    : mpTimerListener(nullptr)
{
}

void AbstractTimer::registerSingleListener(ITimerListener& listener)
{
    mpTimerListener = &listener;
}
