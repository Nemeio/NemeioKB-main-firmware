/*
 * BQ24296Charger.h
 *
 *  Created on: Apr 10, 2018
 *      Author: amongatcheucheu
 */

#ifndef BATTERYCHARGER_BQ24296CHARGER_BQ24296CHARGER_H_
#define BATTERYCHARGER_BQ24296CHARGER_BQ24296CHARGER_H_

#include "stdint.h"
#include "iI2C.h"
#include "IBatteryCharger.h"
#include "Module.hpp"

#define BQ24296_I2C_ADDRESS 0x6B

#define INPUT_SOURCE_CTRL_REG 0x00
#define POWER_ON_CTRL_REG 0x01
#define POWER_ON_CTRL_CHG_CONFIG_MASK (1 << 4)
#define CHARGE_CURRENT_CTRL_REG 0x02
#define PRECHARGE_TERMINATION_CURRENT_CTRL_REG 0x03
#define CHARGE_VOLTAGE_CTRL_REG 0x04
#define PRECHARGE_TERMINATION_VOLTAGE_CTRL_REG 0x05
#define PRECHARGE_TERMINATION_VOLTAGE_CTRL_WD_MASK (3 << 4)
#define BOOST_THERMAL_CTRL_REG 0x06
#define MISC_OPERATION_CTRL_REG 0x07
#define SYSTEM_STATUS_REG 0x08
#define NEW_FAULT_REG 0x09
#define VENDOR_REG 0x0A

class BQ24296Charger : public IBatteryCharger,
                       public Module<Identification::ModuleId::BATTERY_CHARGER>
{
public:
    BQ24296Charger(iI2C& iI2c);
    ~BQ24296Charger() final = default;

    void init();
    uint32_t getVendorId() final;

    double getChargeVoltage() final;
    double getChargeCurrent() final;
    double getInputCurrentLimit() final;
    double getInputVoltageLimit() final;

    void enableCharge() final;
    void disableCharge() final;

    void setChargeVoltage(double voltage) final;
    void setChargeCurrent(double current) final;
    void setInputCurrentLimit(double current) final;
    void setInputVoltageLimit(double voltage) final;

    static constexpr uint32_t BQ24296_VENDOR_ID = 0x01;
    static constexpr uint32_t BQ24297_VENDOR_ID = 0x03;

private:
    static constexpr uint8_t convertPrechargeTerminationCurrent(uint16_t currentMilliAmp);
    static constexpr uint8_t convertCurrentsToPrechargeTerminationCurrentReg(
        uint16_t preChargeCurrentLimitMilliAmp, uint16_t terminationCurrentLimitMilliAmp);
    static constexpr uint16_t PRECHARGE_TERMINATION_CURRENT_MILLIAMP_OFFSET = 128;
    static constexpr uint16_t PRECHARGE_TERMINATION_CURRENT_MILLIAMP_MULTIPLIER = 128;
    static constexpr uint16_t MIN_PRECHARGE_TERMINATION_CURRENT_MILLIAMP =
        PRECHARGE_TERMINATION_CURRENT_MILLIAMP_OFFSET;
    static constexpr uint16_t MAX_PRECHARGE_TERMINATION_CURRENT_MILLIAMP =
        PRECHARGE_TERMINATION_CURRENT_MILLIAMP_OFFSET
        + 0x0F * PRECHARGE_TERMINATION_CURRENT_MILLIAMP_MULTIPLIER;

    static constexpr uint16_t PRECHARGE_CURRENT_lIMIT_MILLIAMP = 256;
    static constexpr uint16_t TERMINATION_CURRENT_LIMIT_MILLIAMP = 128;

    iI2C& mI2C;

    void setChargeState(bool newState);
    bool readReg(uint16_t reg, uint8_t* value, size_t size);
    bool writeReg(uint16_t reg, uint8_t* value, size_t size);
};

#endif /* BATTERYCHARGER_BQ24296CHARGER_BQ24296CHARGER_H_ */
