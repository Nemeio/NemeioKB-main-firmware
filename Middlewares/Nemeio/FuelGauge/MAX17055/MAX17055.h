/*
 * MAX17055.h
 *
 *  Created on: May 17, 2018
 *      Author: amongatcheucheu
 */

#ifndef FUELGAUGE_MAX17055_MAX17055_H_
#define FUELGAUGE_MAX17055_MAX17055_H_

#include "iI2C.h"
#include "IFuelGauge.h"
#include "Module.hpp"
#include "ISystem.hpp"
#include "fs.hpp"

#define MAX17055_I2C_ADDRESS (0x36)
/*
 * This macro converts empty voltage target (VE) and recovery voltage (VR)
 * in mV to max17055 0x3a reg value. max17055 declares 0% (empty battery) at
 * VE. max17055 reenables empty detection when the cell voltage rises above VR.
 * VE ranges from 0 to 5110mV, and VR ranges from 0 to 5080mV.
 */
#define MAX17055_VEMPTY_CONV(ve_mv, vr_mv) (((ve_mv / 10) << 7) | (vr_mv / 40))

enum max17055_register
{
    MAX17055_STATUS_REG = 0x00,
    MAX17055_VALRTTH_REG = 0x01,
    MAX17055_TALRTTH_REG = 0x02,
    MAX17055_SALRTTH_REG = 0x03,
    MAX17055_REPCAP_REG = 0x05,
    MAX17055_REPSOC_REG = 0x06,
    MAX17055_AGE_REG = 0x07,
    MAX17055_TEMP_REG = 0x08,
    MAX17055_VCELL_REG = 0x09,
    MAX17055_CURRENT_REG = 0x0A,
    MAX17055_AVGCURRENT_REG = 0x0B,
    MAX17055_MIXSOC_REG = 0x0D,
    MAX17055_MIXCAP_REG = 0x0F,

    MAX17055_FULLCAPREP_REG = 0x10,
    MAX17055_TTE_REG = 0X11,
    MAX17055_QRTABLE00_REG = 0x12,
    MAX17055_FULLSOCTHR_REG = 0x13,
    MAX17055_CYCLES_REG = 0x17,
    MAX17055_DESIGNCAP_REG = 0x18,
    MAX17055_AVGVCELL_REG = 0x19,
    MAX17055_MAXMINVOLT_REG = 0x1B,
    MAX17055_CONFIG_REG = 0x1D,
    MAX17055_ICHGTERM_REG = 0x1E,

    MAX17055_TTF_REG = 0x20,
    MAX17055_VERSION_REG = 0x21,
    MAX17055_QRTABLE10_REG = 0x22,
    MAX17055_FULLCAPNOM_REG = 0x23,
    MAX17055_LEARNCFG_REG = 0x28,
    MAX17055_RELAXCFG_REG = 0x2A,
    MAX17055_TGAIN_REG = 0x2C,
    MAX17055_TOFF_REG = 0x2D,

    MAX17055_QRTABLE20_REG = 0x32,
    MAX17055_RCOMP0_REG = 0x38,
    MAX17055_TEMPCO_REG = 0x39,
    MAX17055_VEMPTY_REG = 0x3A,
    MAX17055_FSTAT_REG = 0x3D,

    MAX17055_QRTABLE30_REG = 0x42,
    MAX17055_DQACC_REG = 0x45,
    MAX17055_DPACC_REG = 0x46,
    MAX17055_VFSOC0_REG = 0x48,
    MAX17055_QH0_REG = 0x4C,
    MAX17055_QH_REG = 0x4D,

    MAX17055_VFSOC0_QH0_LOCK_REG = 0x60,
    MAX17055_LOCK1_REG = 0x62,
    MAX17055_LOCK2_REG = 0x63,

    MAX17055_MODELDATA_START_REG = 0x80,

    MAX17055_IALRTTH_REG = 0xB4,
    MAX17055_CURVE_REG = 0xB9,
    MAX17055_HIBCFG_REG = 0xBA,
    MAX17055_CONFIG2_REG = 0xBB,

    MAX17055_MODELCFG_REG = 0xDB,

    MAX17055_OCV_REG = 0xFB,
    MAX17055_VFSOC_REG = 0xFF,
};

class MAX17055 : public IFuelGauge, public Module<Identification::ModuleId::FUELGAUGE>
{
public:
    MAX17055(iI2C& iI2c, ISystem& system);
    ~MAX17055() final = default;
    bool init() final;
    bool configure(fs::File& file) final;
    bool getBatteryInformation(BatteryInformation& batteryInformation) final;
    bool updateLearnedParameters(fs::File& file) final;

    int16_t getCurrentMa() final;
    uint16_t getVoltageMv() final;

private:
    static constexpr uint32_t MAGIC_NUMBER = 0xF6BA77F6;
    struct LearnedParameters
    {
        uint32_t magicNumber; // Make sure the file we are loading contains the correct data
        uint16_t RCOMP0;
        uint16_t TempCo;
        uint16_t FullCapRep;
        uint16_t Cycles;
        uint16_t FullCapNom;
    };

    LearnedParameters mLearnedParameters;

    iI2C& mI2C;
    ISystem& mSystem;
    bool mbInitialized;

    static constexpr uint16_t RSENSE_MILLIOHM = 10;
    static constexpr uint16_t CAPACITY_MILLIAMPH = 2000;
    static constexpr uint16_t CHARGE_TERMINATION_CURRENT_MILLIAMP = 160;
    static constexpr uint16_t VE_TARGET_VOLTAGE_MILLIVOLT = 3100;
    static constexpr uint16_t VR_RECOVERY_VOLTAGE_MILLIVOLT = 3880;

    static constexpr uint16_t CHARGE_VOLTAGE_MILLIVOLT = 4275;

    static constexpr uint32_t DELAY_WAIT_WRITE_VERIFY_MS = 1;
    static constexpr uint32_t DELAY_POLL_REG_MS = 10;
    static constexpr uint32_t DELAY_LOAD_LEARNED_PARAMS_MS = 350;
    static constexpr uint8_t WRITE_VERIFY_MAX_ATTEMPT = 3;
    static constexpr uint8_t POLL_MAX_ATTEMPT = 75;

    static constexpr uint16_t DPACC_REG_VALUE = 0x0C80;

    static constexpr uint16_t STATUS_POR_MASK = 1 << 1;
    static constexpr uint16_t STATUS_BATTERY_STATUS_MASK = 1 << 3;
    static constexpr uint16_t CYCLE_64PERCENT_CHANGE_MASK = 1 << 6;
    static constexpr uint16_t FSTAT_DNR_MASK = 1 << 0;
    static constexpr uint16_t MODELCFG_MODELREFRESH_MASK = 1 << 15;

    static constexpr float PERCENTAGE_MULTIPLIER = 1.0 / 256;
    static constexpr float CAPACITY_MULTIPLIER = 5.0 / RSENSE_MILLIOHM;
    static constexpr float VOLTAGE_MULTIPLIER = 1.25 / 16;
    static constexpr float CURRENT_MULTIPLIER = 1.5625 / RSENSE_MILLIOHM;

    static constexpr float TIME_MULTIPLIER = 5.625;
    static constexpr uint16_t TIME_INFINITE_REG_VALUE = UINT16_MAX;
    static constexpr uint32_t TIME_INFINITE_VALUE = UINT32_MAX;

    static constexpr uint16_t convertToDesignCapRegVal(uint16_t capacityMilliAmpH);
    static constexpr uint16_t convertToDQAccRegVal(uint16_t designCapRegVal);
    static constexpr uint16_t convertToIchgTermRegVal(uint16_t chargeTerminationCurrentMilliAmp);
    static constexpr uint16_t convertToVEmptyRegVal(uint16_t veTargetVoltageMilliVolt,
                                                    uint16_t vrRecoveryVoltageMilliVolt);
    static constexpr uint32_t convertTimeRegValToTimeVal(uint16_t timeRegVal);

    bool checkForPOR(bool& porStatus);
    bool waitDataReady();
    bool forceExitFromHibernate(uint16_t& hibCfgRegVal);
    bool initEZConfig();
    bool waitModelCfgRefresh();
    bool restoreHibCfg(uint16_t hibCfgRegVal);

    bool reinitParameters(fs::File& file);
    bool initBatteryParameters();
    bool loadLearnedParametersFromFS(fs::File& file);
    bool saveLearnedParametersToFS(fs::File& file);
    bool restoreLearnedParameters(const LearnedParameters& parameters);
    bool readLearnedParametersRegisters(LearnedParameters& parameters);
    bool resetPORStatus();
    bool isBatteryConnected();
    bool isLearnedParametersSaveNecessary(uint16_t previousCyclesRegValue,
                                          uint16_t newCyclesRegValue);

    bool writeRegister(uint8_t reg, uint16_t value);
    bool writeAndVerifyRegister(uint8_t reg, uint16_t value);
    bool readRegister(uint8_t reg, uint16_t& value);
    bool pollRegisterClearedFlag(uint8_t reg, uint16_t mask);

    bool getReportedStateOfCharge(uint8_t& stateOfChargePercent);
    bool getRemainingCapacity(uint16_t& remainingCapacityMilliAh);
    bool getTimeToFull(uint32_t& timeToFullSec);
    bool getTimeToEmpty(uint32_t& timeToEmptySec);
    bool getCyclesHundredth(uint16_t& cyclesHundredth);
    bool getAge(uint8_t& agePercent);
};

#endif /* FUELGAUGE_MAX17055_MAX17055_H_ */
