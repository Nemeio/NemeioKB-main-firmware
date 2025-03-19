/*
 * BatteryInformation.hpp
 *
 *  Created on: 6 juil. 2020
 *      Author: eskoric
 */

#ifndef BATTERY_BATTERYINFORMATION_HPP_
#define BATTERY_BATTERYINFORMATION_HPP_

#include <cstdint>

struct BatteryInformation
{
    uint8_t stateOfChargePercent;
    uint16_t remainingCapacityMilliAh;
    uint32_t timeToFullSec;
    uint32_t timeToEmptySec;
    uint16_t cyclesHundredth;
    uint8_t agePercent;

    BatteryInformation(uint8_t stateOfChargePercent_,
                       uint16_t remainingCapacityMilliAh_,
                       uint32_t timeToFullSec_,
                       uint32_t timeToEmptySec_,
                       uint16_t cyclesHundredth_,
                       uint8_t agePercent_)
        : stateOfChargePercent(stateOfChargePercent_)
        , remainingCapacityMilliAh(remainingCapacityMilliAh_)
        , timeToFullSec(timeToFullSec_)
        , timeToEmptySec(timeToEmptySec_)
        , cyclesHundredth(cyclesHundredth_)
        , agePercent(agePercent_)
    {
    }

    BatteryInformation()
        : BatteryInformation(0, 0, 0, 0, 0, 0)
    {
    }
};

#endif /* BATTERY_BATTERYINFORMATION_HPP_ */
