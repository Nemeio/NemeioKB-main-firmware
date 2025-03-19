/*
 * Signal.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: Labo
 */

#include "Signal.hpp"

bool Signal::wait(DelayMs delay)
{
    uint8_t msg;
    return mDataSignal.waitData(delay, msg);
}

void Signal::setFromISR()
{
    uint8_t msg = 0;
    return mDataSignal.setFromISRData(msg);
}

void Signal::set(DelayMs delay)
{
    uint8_t msg = 0;
    return mDataSignal.setData(delay, msg);
}

void Signal::set()
{
    uint8_t msg = 0;
    return mDataSignal.setData(msg);
}

void Signal::clear()
{
    return mDataSignal.clear();
}
