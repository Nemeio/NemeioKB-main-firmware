/*
 * IBLECommStrategy.hpp
 *
 *  Created on: Nov 22, 2019
 *      Author: Labo
 */

#ifndef BLE_IBLECOMMSTRATEGY_HPP_
#define BLE_IBLECOMMSTRATEGY_HPP_

#include "SystemTypes.hpp"

class IBLECommStrategy
{
public:
    IBLECommStrategy() = default;
    virtual ~IBLECommStrategy() = default;

    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual void resetHostReadyState() = 0;
    virtual bool waitForHostReady(DelayMs delay) = 0;
    virtual DelayMs getPollDelay() = 0;
    virtual bool isDataToSend() = 0;
};

#endif /* BLE_IBLECOMMSTRATEGY_HPP_ */
