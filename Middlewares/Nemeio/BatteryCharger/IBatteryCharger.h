/*
 * IBatteryCharger.h
 *
 *  Created on: Apr 10, 2018
 *      Author: amongatcheucheu
 */

#ifndef NEMEIO_BATTERYCHARGER_IBATTERYCHARGER_H_
#define NEMEIO_BATTERYCHARGER_IBATTERYCHARGER_H_

class IBatteryCharger
{
public:
    enum class TechErrorCode
    {
        COMM_ERROR = 1,
    };
    IBatteryCharger() = default;
    virtual ~IBatteryCharger() = default;

    virtual void init() = 0;
    virtual uint32_t getVendorId() = 0;

    virtual void enableCharge() = 0;
    virtual void disableCharge() = 0;

    virtual double getChargeVoltage() = 0;
    virtual double getChargeCurrent() = 0;
    virtual double getInputCurrentLimit() = 0;
    virtual double getInputVoltageLimit() = 0;

    virtual void setChargeVoltage(double voltage) = 0;
    virtual void setChargeCurrent(double current) = 0;
    virtual void setInputCurrentLimit(double current) = 0;
    virtual void setInputVoltageLimit(double voltage) = 0;
};

#endif /* NEMEIO_BATTERYCHARGER_IBATTERYCHARGER_H_ */
