/*
 * IBattery.hpp
 *
 *  Created on: 7 juil. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_BATTERY_IBATTERY_HPP_
#define NEMEIO_BATTERY_IBATTERY_HPP_

#include "BatteryInformation.hpp"

class IBattery
{
public:
    enum class RetCode
    {
        SUCCESS,
        INFORMATION_NOT_READY,
        FUELGAUGE_ERROR
    };
    IBattery() = default;
    virtual ~IBattery() = default;

    virtual RetCode getBatteryInformation(BatteryInformation& batteryInformation) = 0;
};

#endif /* NEMEIO_BATTERY_IBATTERY_HPP_ */
