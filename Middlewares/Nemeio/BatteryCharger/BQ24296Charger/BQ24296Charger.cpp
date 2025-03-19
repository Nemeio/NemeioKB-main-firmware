/*
 * BQ24296Charger.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: amongatcheucheu
 */

#include "BQ24296Charger.h"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "bq24296"

BQ24296Charger::BQ24296Charger(iI2C& iI2c)
    : mI2C(iI2c)
{
}

void BQ24296Charger::init()
{
    bool ret = false;
    uint8_t regValueU8 = 0;
    uint32_t vendorId = getVendorId();
    LOG_INFO(MODULE, "VENDOR " << etl::showbase << etl::hex << vendorId);
    if(vendorId != 0)
    {
        ret = readReg(INPUT_SOURCE_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "INPUT_SOURCE_CTRL " << etl::showbase << etl::hex << regValueU8);
        }

        ret = readReg(POWER_ON_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "POWER_ON_CTRL " << etl::showbase << etl::hex << regValueU8);
        }

        ret = readReg(CHARGE_CURRENT_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "CHARGE_CURRENT_CTRL " << etl::showbase << etl::hex << regValueU8);
        }

        // Set pre-charge & termination currents
        regValueU8 =
            convertCurrentsToPrechargeTerminationCurrentReg(PRECHARGE_CURRENT_lIMIT_MILLIAMP,
                                                            TERMINATION_CURRENT_LIMIT_MILLIAMP);
        writeReg(PRECHARGE_TERMINATION_CURRENT_CTRL_REG, &regValueU8, sizeof(regValueU8));

        ret = readReg(PRECHARGE_TERMINATION_CURRENT_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE,
                     "PRECHARGE_TERMINATION_CURRENT_CTRL " << etl::showbase << etl::hex
                                                           << regValueU8);
        }

        ret = readReg(CHARGE_VOLTAGE_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "CHARGE_VOLTAGE_CTRL " << etl::showbase << etl::hex << regValueU8);
        }

        ret = readReg(PRECHARGE_TERMINATION_VOLTAGE_CTRL_REG, &regValueU8, sizeof(regValueU8));
        // Disable battery charger watchdog
        regValueU8 &= ~PRECHARGE_TERMINATION_VOLTAGE_CTRL_WD_MASK;
        writeReg(PRECHARGE_TERMINATION_VOLTAGE_CTRL_REG, &regValueU8, sizeof(regValueU8));

        ret = readReg(PRECHARGE_TERMINATION_VOLTAGE_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE,
                     "PRECHARGE_TERMINATION_VOLTAGE_CTRL " << etl::showbase << etl::hex
                                                           << regValueU8);
        }

        ret = readReg(BOOST_THERMAL_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "BOOST_THERMAL_CTRL " << etl::showbase << etl::hex << regValueU8);
        }

        ret = readReg(MISC_OPERATION_CTRL_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "MISC_OPERATION_CTRL " << etl::showbase << etl::hex << regValueU8);
        }

        ret = readReg(SYSTEM_STATUS_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "SYSTEM_STATUS " << etl::showbase << etl::hex << regValueU8);
        }

        ret = readReg(NEW_FAULT_REG, &regValueU8, sizeof(regValueU8));
        if(ret)
        {
            LOG_INFO(MODULE, "NEW_FAULT " << etl::showbase << etl::hex << regValueU8);
        }
    }
}

double BQ24296Charger::getChargeVoltage()
{
    return 0;
}
double BQ24296Charger::getChargeCurrent()
{
    return 0;
}
double BQ24296Charger::getInputCurrentLimit()
{
    return 0;
}
double BQ24296Charger::getInputVoltageLimit()
{
    return 0;
}

void BQ24296Charger::setChargeVoltage(double current)
{
    // TODO Implement this function
}
void BQ24296Charger::setChargeCurrent(double current)
{
    // TODO Implement this function
}
void BQ24296Charger::setInputCurrentLimit(double current)
{
    // TODO Implement this function
}
void BQ24296Charger::setInputVoltageLimit(double voltage)
{
    // TODO Implement this function
}

constexpr uint8_t BQ24296Charger::convertPrechargeTerminationCurrent(uint16_t currentMilliAmp)
{
    if(currentMilliAmp < MIN_PRECHARGE_TERMINATION_CURRENT_MILLIAMP
       || currentMilliAmp > MAX_PRECHARGE_TERMINATION_CURRENT_MILLIAMP)
    {
        return 0;
    }

    return static_cast<uint8_t>((currentMilliAmp - PRECHARGE_TERMINATION_CURRENT_MILLIAMP_OFFSET)
                                / PRECHARGE_TERMINATION_CURRENT_MILLIAMP_MULTIPLIER);
}

constexpr uint8_t BQ24296Charger::convertCurrentsToPrechargeTerminationCurrentReg(
    uint16_t preChargeCurrentLimitMilliAmp, uint16_t terminationCurrentLimitMilliAmp)
{
    return static_cast<uint8_t>(
               (convertPrechargeTerminationCurrent(preChargeCurrentLimitMilliAmp) & 0x0F) << 4)
           | (convertPrechargeTerminationCurrent(terminationCurrentLimitMilliAmp) & 0x0F);
}

uint32_t BQ24296Charger::getVendorId()
{
    uint8_t vendorId = 0;
    if(readReg(VENDOR_REG, &vendorId, sizeof(vendorId)))
    {
        vendorId = (vendorId >> 5) & 0x07;
    }
    else
    {
        vendorId = 0;
    }

    return vendorId;
}

void BQ24296Charger::enableCharge()
{
    setChargeState(true);
}

void BQ24296Charger::disableCharge()
{
    setChargeState(false);
}

void BQ24296Charger::setChargeState(bool newState)
{
    uint8_t regValueU8 = 0;
    bool ret = readReg(POWER_ON_CTRL_REG, &regValueU8, sizeof(regValueU8));

    if(newState)
    {
        regValueU8 |= POWER_ON_CTRL_CHG_CONFIG_MASK;
    }
    else
    {
        regValueU8 &= ~POWER_ON_CTRL_CHG_CONFIG_MASK;
    }
    writeReg(POWER_ON_CTRL_REG, &regValueU8, sizeof(regValueU8));

    uint8_t expectedRegValue = regValueU8;

    ret = readReg(POWER_ON_CTRL_REG, &regValueU8, sizeof(regValueU8));

    if(ret && expectedRegValue != regValueU8)
    {
        LOG_ERR(MODULE, "Error setting charge state to " << newState);
    }
}

bool BQ24296Charger::readReg(uint16_t reg, uint8_t* value, size_t size)
{
    II2CErrCode i2cErr = mI2C.I2CReadMem(BQ24296_I2C_ADDRESS, reg, value, size);
    bool ret = (i2cErr == II2C_OK);

    if(!ret)
    {
        LOG_ERR(MODULE,
                "Read register " << etl::showbase << etl::hex << reg << " error "
                                 << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                       static_cast<uint16_t>(
                                                                           TechErrorCode::COMM_ERROR))
                                 << " (i2c " << i2cErr << ")");
    }

    return ret;
}

bool BQ24296Charger::writeReg(uint16_t reg, uint8_t* value, size_t size)
{
    II2CErrCode i2cErr = mI2C.I2CWriteMem(BQ24296_I2C_ADDRESS, reg, value, size);
    bool ret = (i2cErr == II2C_OK);

    if(!ret)
    {
        LOG_ERR(MODULE,
                "Write register " << etl::showbase << etl::hex << reg << " error "
                                  << TechnicalErrorDefs::TechnicalError(
                                         getModuleId(),
                                         static_cast<uint16_t>(TechErrorCode::COMM_ERROR))
                                  << " (i2c " << i2cErr << ")");
    }

    return ret;
}