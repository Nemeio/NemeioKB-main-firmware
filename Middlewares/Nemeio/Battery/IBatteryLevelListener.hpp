/*
 * IBatteryLevelListener.hpp
 *
 *  Created on: Jul 16, 2018
 *      Author: Labo
 */

#ifndef BATTERY_IBATTERYLEVELLISTENER_HPP_
#define BATTERY_IBATTERYLEVELLISTENER_HPP_

#include <stdint.h>

class IBatteryLevelListener
{
public:
    IBatteryLevelListener() = default;
    virtual ~IBatteryLevelListener() = default;

    virtual void batteryLevel(uint8_t batteryLevelPercent) = 0;
};

#endif /* BATTERY_IBATTERYLEVELLISTENER_HPP_ */
