/*
 * MAX17055.cpp
 *
 *  Created on: May 17, 2018
 *      Author: amongatcheucheu
 */

#include "MAX17055.h"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "max17055"

MAX17055::MAX17055(iI2C& iI2c, ISystem& system)
    : mI2C(iI2c)
    , mSystem(system)
    , mbInitialized(false)
{
}

bool MAX17055::init()
{
    bool bPOR = true;
    /* Check if COM is ok */
    bool ret = checkForPOR(bPOR);

    if(ret)
    {
        LOG_INFO(MODULE, "Init success");
    }
    else
    {
        LOG_ERR(MODULE, "Init failed");
    }

    return ret;
}


bool MAX17055::configure(fs::File& file)
{
    bool bPOR = true;
    bool bRet = checkForPOR(bPOR);

    if(bRet)
    {
        if(bPOR)
        {
            bRet = reinitParameters(file) && resetPORStatus();
        }
        else
        {
            bRet = readLearnedParametersRegisters(mLearnedParameters);
        }
    }

    mbInitialized = bRet;

    LOG_INFO(MODULE, "Parameters: ");
    LOG_INFO(MODULE, "magicNumber " << etl::showbase << etl::hex << mLearnedParameters.magicNumber);
    LOG_INFO(MODULE, "RCOMP0 " << etl::showbase << etl::hex << mLearnedParameters.RCOMP0);
    LOG_INFO(MODULE, "TempCo " << etl::showbase << etl::hex << mLearnedParameters.TempCo);
    LOG_INFO(MODULE, "FullCapRep " << etl::showbase << etl::hex << mLearnedParameters.FullCapRep);
    LOG_INFO(MODULE, "Cycles " << etl::showbase << etl::hex << mLearnedParameters.Cycles);
    LOG_INFO(MODULE, "FullCapNom " << etl::showbase << etl::hex << mLearnedParameters.FullCapNom);

    if(bRet)
    {
        LOG_INFO(MODULE, "Configure success");
    }
    else
    {
        LOG_ERR(MODULE, "Configure failed");
    }

    return bRet;
}


bool MAX17055::getBatteryInformation(BatteryInformation& batteryInformation)
{
    if(!mbInitialized)
    {
        return false;
    }

    bool bRet = isBatteryConnected()
                && getReportedStateOfCharge(batteryInformation.stateOfChargePercent)
                && getRemainingCapacity(batteryInformation.remainingCapacityMilliAh)
                && getTimeToFull(batteryInformation.timeToFullSec)
                && getTimeToEmpty(batteryInformation.timeToEmptySec)
                && getCyclesHundredth(batteryInformation.cyclesHundredth)
                && getAge(batteryInformation.agePercent);

    return bRet;
}

bool MAX17055::isBatteryConnected()
{
    uint16_t statusRegValue = 0;
    bool bIsBatteryConnected = false;

    bool bRet = readRegister(MAX17055_STATUS_REG, statusRegValue);
    writeAndVerifyRegister(MAX17055_STATUS_REG, 0);
    if(bRet)
    {
        bIsBatteryConnected = ((statusRegValue & STATUS_BATTERY_STATUS_MASK) == 0);
    }

    return bIsBatteryConnected;
}

bool MAX17055::writeRegister(uint8_t reg, uint16_t value)
{
    II2CErrCode err = II2C_OK;
    uint8_t regValue[2] = {0};
    regValue[0] = value & 0xFF;
    regValue[1] = value >> 8;
    err = mI2C.I2CWriteMem(MAX17055_I2C_ADDRESS, reg, regValue, 2);

    bool bRet = (II2C_OK == err);
    if(!bRet)
    {
        LOG_ERR(MODULE,
                "Write register " << etl::showbase << etl::hex << reg << " error "
                                  << TechnicalErrorDefs::TechnicalError(
                                         getModuleId(),
                                         static_cast<uint16_t>(TechErrorCode::COMM_ERROR)));
    }

    return bRet;
}

bool MAX17055::writeAndVerifyRegister(uint8_t reg, uint16_t value)
{
    bool bSuccess = false;
    uint8_t remainingAttempt = WRITE_VERIFY_MAX_ATTEMPT;
    bool bI2C = false;

    do
    {
        uint16_t valueRead = 0;
        bI2C = writeRegister(reg, value);

        mSystem.delay(DelayMs(DELAY_WAIT_WRITE_VERIFY_MS));

        if(bI2C)
        {
            bI2C = readRegister(reg, valueRead);
        }

        if(bI2C && valueRead == value)
        {
            bSuccess = true;
        }
    } while(bI2C && !bSuccess && (--remainingAttempt != 0));

    if((remainingAttempt == 0) && bI2C && !bSuccess)
    {
        LOG_ERR(MODULE,
                "Write/verify register "
                    << etl::showbase << etl::hex << reg << " error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(
                                                              TechErrorCode::TIMEOUT)));
    }

    return bSuccess;
}

constexpr uint16_t MAX17055::convertToDesignCapRegVal(uint16_t capacityMilliAmpH)
{
    return capacityMilliAmpH * RSENSE_MILLIOHM / 5;
}

constexpr uint16_t MAX17055::convertToIchgTermRegVal(uint16_t chargeTerminationCurrentMilliAmp)
{
    return ((chargeTerminationCurrentMilliAmp * RSENSE_MILLIOHM) << 4) / 25;
}

constexpr uint16_t MAX17055::convertToVEmptyRegVal(uint16_t veTargetVoltageMilliVolt,
                                                   uint16_t vrRecoveryVoltageMilliVolt)
{
    return ((veTargetVoltageMilliVolt / 10) << 7) | (vrRecoveryVoltageMilliVolt / 40);
}

constexpr uint16_t MAX17055::convertToDQAccRegVal(uint16_t designCapRegVal)
{
    return designCapRegVal >> 5;
}

bool MAX17055::initBatteryParameters()
{
    uint16_t savedHibCfgRegVal = 0;

    bool bRet = waitDataReady() && forceExitFromHibernate(savedHibCfgRegVal) && initEZConfig()
                && waitModelCfgRefresh() && restoreHibCfg(savedHibCfgRegVal);

    return bRet;
}

bool MAX17055::restoreLearnedParameters(const LearnedParameters& parameters)
{
    bool bRet = writeAndVerifyRegister(MAX17055_RCOMP0_REG, parameters.RCOMP0)
                && writeAndVerifyRegister(MAX17055_TEMPCO_REG, parameters.TempCo)
                && writeAndVerifyRegister(MAX17055_FULLCAPNOM_REG, parameters.FullCapNom);

    if(parameters.magicNumber != MAGIC_NUMBER)
    {
        return false;
    }

    uint16_t fullCapNom = 0;
    uint16_t mixSoc = 0;
    uint16_t dQAcc = parameters.FullCapNom / 16;

    if(bRet)
    {
        mSystem.delay(DelayMs(DELAY_LOAD_LEARNED_PARAMS_MS));

        bRet = readRegister(MAX17055_FULLCAPNOM_REG, fullCapNom)
               && readRegister(MAX17055_MIXSOC_REG, mixSoc);
    }

    if(bRet)
    {
        uint16_t mixCap = mixSoc * fullCapNom / 25600;
        bRet = writeAndVerifyRegister(MAX17055_MIXCAP_REG, mixCap)
               && writeAndVerifyRegister(MAX17055_FULLCAPREP_REG, parameters.FullCapRep)
               && writeAndVerifyRegister(MAX17055_DPACC_REG, DPACC_REG_VALUE)
               && writeAndVerifyRegister(MAX17055_DQACC_REG, dQAcc);
    }

    if(bRet)
    {
        mSystem.delay(DelayMs(DELAY_LOAD_LEARNED_PARAMS_MS));

        bRet = writeAndVerifyRegister(MAX17055_CYCLES_REG, parameters.Cycles);
    }

    return bRet;
}

bool MAX17055::resetPORStatus()
{
    uint16_t status = 0;
    bool bRet = readRegister(MAX17055_STATUS_REG, status)
                && writeAndVerifyRegister(MAX17055_STATUS_REG, status & ~(STATUS_POR_MASK));

    return bRet;
}

bool MAX17055::checkForPOR(bool& porStatus)
{
    uint16_t statusRegVal = 0;
    bool ret = readRegister(MAX17055_STATUS_REG, statusRegVal);

    if(ret)
    {
        porStatus = (statusRegVal & STATUS_POR_MASK) != 0;
    }

    return ret;
}

bool MAX17055::reinitParameters(fs::File& file)
{
    bool bRet = initBatteryParameters();

    /* Whever loading from FS is working or not, component is initialized.
	 * So we don't take the return value into account */
    if(bRet && loadLearnedParametersFromFS(file))
    {
        restoreLearnedParameters(mLearnedParameters);
    }

    return bRet;
}

bool MAX17055::updateLearnedParameters(fs::File& file)
{
    if(!mbInitialized)
    {
        return false;
    }

    uint16_t previousCycleRegValue = mLearnedParameters.Cycles;

    bool bRet = readLearnedParametersRegisters(mLearnedParameters);
    uint16_t designCapReg = 0;
    bRet = readRegister(MAX17055_DESIGNCAP_REG, designCapReg);

    if(bRet && isLearnedParametersSaveNecessary(previousCycleRegValue, mLearnedParameters.Cycles))
    {
        bRet = saveLearnedParametersToFS(file);
    }

    return bRet;
}

bool MAX17055::loadLearnedParametersFromFS(fs::File& file)
{
    return (file.seek(0, fs::SeekFlag::ABSOLUTE_POS) == 0)
           && file.read(&mLearnedParameters, sizeof(mLearnedParameters))
                  == sizeof(mLearnedParameters);
}

bool MAX17055::saveLearnedParametersToFS(fs::File& file)
{
    bool bRet = (file.truncate(0) == fs::FS_ERR_OK) && file.seek(0, fs::SeekFlag::ABSOLUTE_POS) == 0
                && file.write(&mLearnedParameters, sizeof(mLearnedParameters))
                       == sizeof(mLearnedParameters);

    file.sync();

    return bRet;
}

bool MAX17055::isLearnedParametersSaveNecessary(uint16_t previousCyclesRegValue,
                                                uint16_t newCyclesRegValue)
{
    // It is recommended to save the learned capacity parameters every time bit 6 of the Cycles register toggles
    return (previousCyclesRegValue & CYCLE_64PERCENT_CHANGE_MASK)
           != (newCyclesRegValue & CYCLE_64PERCENT_CHANGE_MASK);
}

bool MAX17055::readLearnedParametersRegisters(LearnedParameters& parameters)
{
    bool bRet = readRegister(MAX17055_RCOMP0_REG, parameters.RCOMP0)
                && readRegister(MAX17055_TEMPCO_REG, parameters.TempCo)
                && readRegister(MAX17055_FULLCAPREP_REG, parameters.FullCapRep)
                && readRegister(MAX17055_CYCLES_REG, parameters.Cycles)
                && readRegister(MAX17055_FULLCAPNOM_REG, parameters.FullCapNom);

    if(bRet)
    {
        mLearnedParameters.magicNumber = MAGIC_NUMBER;
    }
    else
    {
        mLearnedParameters.magicNumber = 0;
    }

    return bRet;
}

bool MAX17055::readRegister(uint8_t reg, uint16_t& value)
{
    II2CErrCode err = II2C_OK;
    uint8_t regValue[2] = {0};
    err = mI2C.I2CReadMem(MAX17055_I2C_ADDRESS, reg, regValue, 2);
    value = regValue[0] | (regValue[1] << 8);

    bool bRet = (II2C_OK == err);

    if(!bRet)
    {
        LOG_ERR(MODULE,
                "Read register " << etl::showbase << etl::hex << reg << " error "
                                 << TechnicalErrorDefs::TechnicalError(
                                        getModuleId(),
                                        static_cast<uint16_t>(TechErrorCode::COMM_ERROR)));
    }

    return bRet;
}

bool MAX17055::pollRegisterClearedFlag(uint8_t reg, uint16_t mask)
{
    bool bSuccess = false;
    uint8_t remainingAttempt = POLL_MAX_ATTEMPT;


    bool bI2C = false;
    do
    {
        mSystem.delay(DelayMs(DELAY_POLL_REG_MS));

        uint16_t valueRead = 0;
        bI2C = readRegister(reg, valueRead);

        if(bI2C && (valueRead & mask) == 0)
        {
            bSuccess = true;
        }
    } while(bI2C && !bSuccess && (--remainingAttempt != 0));

    if((remainingAttempt == 0) && bI2C && !bSuccess)
    {
        LOG_ERR(MODULE,
                "Poll register " << etl::showbase << etl::hex << reg << " error "
                                 << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                       static_cast<uint16_t>(
                                                                           TechErrorCode::TIMEOUT)));
    }

    return bSuccess;
}

bool MAX17055::getReportedStateOfCharge(uint8_t& stateOfChargePercent)
{
    uint16_t reportedStateOfCharge = 0;
    bool bRet = readRegister(MAX17055_REPSOC_REG, reportedStateOfCharge);
    if(bRet)
    {
        stateOfChargePercent = static_cast<uint8_t>(reportedStateOfCharge * PERCENTAGE_MULTIPLIER);
    }

    return bRet;
}

bool MAX17055::getRemainingCapacity(uint16_t& remainingCapacityMilliAh)
{
    uint16_t repCapRegValue = 0;
    bool bRet = readRegister(MAX17055_REPCAP_REG, repCapRegValue);
    if(bRet)
    {
        remainingCapacityMilliAh = static_cast<uint16_t>(repCapRegValue * CAPACITY_MULTIPLIER);
    }

    return bRet;
}

bool MAX17055::getTimeToFull(uint32_t& timeToFullSec)
{
    uint16_t timeToFullRegValue = 0;
    bool bRet = readRegister(MAX17055_TTF_REG, timeToFullRegValue);
    if(bRet)
    {
        timeToFullSec = convertTimeRegValToTimeVal(timeToFullRegValue);
    }

    return bRet;
}

bool MAX17055::getTimeToEmpty(uint32_t& timeToEmptySec)
{
    uint16_t timeToEmptyRegValue = 0;
    bool bRet = readRegister(MAX17055_TTE_REG, timeToEmptyRegValue);
    if(bRet)
    {
        timeToEmptySec = convertTimeRegValToTimeVal(timeToEmptyRegValue);
    }

    return bRet;
}

bool MAX17055::getCyclesHundredth(uint16_t& cyclesHundredth)
{
    uint16_t cyclesRegValue = 0;
    bool bRet = readRegister(MAX17055_CYCLES_REG, cyclesRegValue);
    if(bRet)
    {
        cyclesHundredth = cyclesRegValue;
    }

    return bRet;
}

constexpr uint32_t MAX17055::convertTimeRegValToTimeVal(uint16_t timeRegVal)
{
    return TIME_INFINITE_REG_VALUE == timeRegVal
               ? TIME_INFINITE_VALUE
               : static_cast<uint32_t>(timeRegVal * TIME_MULTIPLIER);
}

bool MAX17055::waitDataReady()
{
    return pollRegisterClearedFlag(MAX17055_FSTAT_REG, FSTAT_DNR_MASK);
}

bool MAX17055::forceExitFromHibernate(uint16_t& hibCfgRegVal)
{
    return readRegister(MAX17055_HIBCFG_REG, hibCfgRegVal) && writeRegister(0x60, 0x90)
           && writeRegister(MAX17055_HIBCFG_REG, 0x00) && writeRegister(0x60, 0x00);
}

bool MAX17055::initEZConfig()
{
    constexpr uint16_t designCapRegVal = convertToDesignCapRegVal(CAPACITY_MILLIAMPH);
    constexpr uint16_t ichgTermRegVal = convertToIchgTermRegVal(CHARGE_TERMINATION_CURRENT_MILLIAMP);
    constexpr uint16_t vEmptyRegVal = convertToVEmptyRegVal(VE_TARGET_VOLTAGE_MILLIVOLT,
                                                            VR_RECOVERY_VOLTAGE_MILLIVOLT);
    constexpr uint16_t dQAccRegVal = convertToDQAccRegVal(designCapRegVal);

    bool bRet = writeRegister(MAX17055_DESIGNCAP_REG, designCapRegVal)
                && writeRegister(MAX17055_DQACC_REG, dQAccRegVal)
                && writeRegister(MAX17055_ICHGTERM_REG, ichgTermRegVal)
                && writeRegister(MAX17055_VEMPTY_REG, vEmptyRegVal);

    if(bRet)
    {
        if(CHARGE_VOLTAGE_MILLIVOLT > 4275)
        {
            uint16_t dpacc = dQAccRegVal * 0xC800 / designCapRegVal;
            bRet = writeRegister(MAX17055_DPACC_REG, dpacc)
                   && writeRegister(MAX17055_MODELCFG_REG, 0x8400);
        }
        else
        {
            uint16_t dpacc = dQAccRegVal * 0xAC6A / designCapRegVal;
            bRet = writeRegister(MAX17055_DPACC_REG, dpacc)
                   && writeRegister(MAX17055_MODELCFG_REG, 0x8000);
        }
    }

    return bRet;
}

bool MAX17055::waitModelCfgRefresh()
{
    return pollRegisterClearedFlag(MAX17055_MODELCFG_REG, MODELCFG_MODELREFRESH_MASK);
}

bool MAX17055::restoreHibCfg(uint16_t hibCfgRegVal)
{
    return writeRegister(MAX17055_HIBCFG_REG, hibCfgRegVal);
}

bool MAX17055::getAge(uint8_t& agePercent)
{
    uint16_t ageRegValue = 0;
    bool bRet = readRegister(MAX17055_AGE_REG, ageRegValue);
    if(bRet)
    {
        agePercent = static_cast<uint8_t>(ageRegValue * PERCENTAGE_MULTIPLIER);
    }

    return bRet;
}

int16_t MAX17055::getCurrentMa()
{
    int16_t current = 0;
    bool bRet = readRegister(MAX17055_CURRENT_REG, reinterpret_cast<uint16_t&>(current));

    if(bRet)
    {
        current = (current * CURRENT_MULTIPLIER);
    }

    return current;
}

uint16_t MAX17055::getVoltageMv()
{
    uint16_t voltage = 0;
    bool bRet = readRegister(MAX17055_VCELL_REG, voltage);

    if(!bRet)
    {
        voltage = 0;
    }

    return voltage * VOLTAGE_MULTIPLIER;
}
