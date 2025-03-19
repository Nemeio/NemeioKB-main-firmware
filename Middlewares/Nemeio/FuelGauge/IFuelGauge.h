/*
 * IFuelGauge.h
 *
 *  Created on: May 17, 2018
 *      Author: amongatcheucheu
 */

#ifndef FUELGAUGE_IFUELGAUGE_H_
#define FUELGAUGE_IFUELGAUGE_H_

#include "BatteryInformation.hpp"
#include "fs.hpp"

class IFuelGauge
{
public:
    enum class TechErrorCode
    {
        COMM_ERROR = 1,
        TIMEOUT
    };

    IFuelGauge() = default;
    virtual ~IFuelGauge() = default;
    virtual bool init() = 0;
    virtual bool configure(fs::File& file) = 0;
    virtual bool getBatteryInformation(BatteryInformation& batteryInformation) = 0;
    virtual bool updateLearnedParameters(fs::File& file) = 0;

    virtual int16_t getCurrentMa() = 0;
    virtual uint16_t getVoltageMv() = 0;
};

#endif /* FUELGAUGE_IFUELGAUGE_H_ */
