#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MAX17055.h"
#include "MockI2C.hpp"
#include "MockSystem.hpp"
#include "MockFileSystem.hpp"
#include "gtest_helpers.hpp"
#include "fs.hpp"

using ::testing::_;
using ::testing::Args;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrictMock;

class MAX17055Test : public ::testing::Test
{
protected:
    MAX17055Test()
        : fuelGauge(i2c, system)
        , file(fileSystem){};

    void initNoPORNominal()
    {
        constexpr uint16_t BATTERY_CONNECTED_STATUS_REG_VALUE = 0;

        ON_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
            .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(BATTERY_CONNECTED_STATUS_REG_VALUE),
                                 Return(II2C_OK)));

        EXPECT_TRUE(fuelGauge.configure(file));
    }

    void updateLearnedParameters(uint16_t cyclesRegValue, bool bExpectSave)
    {
        ON_CALL(i2c, I2CReadMem(_, MAX17055_CYCLES_REG, _, _))
            .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(cyclesRegValue), Return(II2C_OK)));

        if(bExpectSave)
        {
            EXPECT_CALL(fileSystem, fileTruncate(_, 0)).Times(1).WillOnce(Return(fs::FS_ERR_OK));
            EXPECT_CALL(fileSystem, fileSeek(_, 0, _)).Times(1).WillOnce(Return(0));
            EXPECT_CALL(fileSystem, fileWrite(_, _, FuelGaugeDataSize))
                .Times(1)
                .WillOnce(Return(FuelGaugeDataSize));
            EXPECT_CALL(fileSystem, fileSync).Times(1).WillOnce(Return(fs::FS_ERR_OK));
        }

        EXPECT_TRUE(fuelGauge.updateLearnedParameters(file));
    }

    struct LearnedParameters
    {
        uint32_t magicNumber; // Make sure the file we are loading contains the correct data
        uint16_t RCOMP0;
        uint16_t TempCo;
        uint16_t FullCapRep;
        uint16_t Cycles;
        uint16_t FullCapNom;
    };

    static constexpr ssize_t FuelGaugeDataSize = 16;

    NiceMock<MockI2C> i2c;
    NiceMock<MockSystem> system;
    StrictMock<MockFileSystem> fileSystem;
    fs::File file;
    MAX17055 fuelGauge;
};

TEST_F(MAX17055Test, InitNoPOR)
{
    initNoPORNominal();
}

TEST_F(MAX17055Test, InitPORNominalFirstStartup_NoExistingConfig)
{
    constexpr uint16_t POR_STATUS_REG_VAL = 1 << 1;
    constexpr uint16_t DEFAULT_STATUS_REG_VAL = 0;
    constexpr uint16_t DATA_READY_FSTAT_REG_VALUE = ~(1 << 0);
    constexpr uint16_t REFRESH_DONE_MODELCFG_REG_VALUE = ~(1 << 15);

    EXPECT_CALL(i2c, I2CReadMem(_, _, _, _)).WillRepeatedly(Return(II2C_OK));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(POR_STATUS_REG_VAL), Return(II2C_OK)))
        .WillRepeatedly(DoAll(SetArgNPointeeToUInt16<2>(DEFAULT_STATUS_REG_VAL), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_FSTAT_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(DATA_READY_FSTAT_REG_VALUE), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_MODELCFG_REG, _, _))
        .Times(1)
        .WillOnce(
            DoAll(SetArgNPointeeToUInt16<2>(REFRESH_DONE_MODELCFG_REG_VALUE), Return(II2C_OK)));

    EXPECT_CALL(fileSystem, fileSeek(_, _, fs::SeekFlag::ABSOLUTE_POS)).Times(1).WillOnce(Return(-1));

    EXPECT_TRUE(fuelGauge.configure(file));
}

TEST_F(MAX17055Test, InitPORNominal_ExistingConfig)
{
    constexpr LearnedParameters params = {
        .magicNumber = 0xF6BA77F6,
        .RCOMP0 = 1,
        .TempCo = 2,
        .FullCapRep = 3,
        .Cycles = 4,
        .FullCapNom = 5,
    };

    constexpr uint16_t POR_STATUS_REG_VAL = 1 << 1;
    constexpr uint16_t DEFAULT_STATUS_REG_VAL = 0;
    constexpr uint16_t DATA_READY_FSTAT_REG_VALUE = ~(1 << 0);
    constexpr uint16_t REFRESH_DONE_MODELCFG_REG_VALUE = ~(1 << 15);
    constexpr uint16_t FULLCAPNOM_REG_VALUE = 0;
    constexpr uint16_t MIXSOC_REG_VALUE = 1;
    constexpr uint16_t RCOMP_REG_VALUE = 0;
    constexpr uint16_t MIXCAP_REG_VALUE = MIXSOC_REG_VALUE * params.FullCapNom / 25600;
    constexpr uint16_t DPACC_REG_VALUE = 0x0C80;
    constexpr uint16_t DQACC_REG_VALUE = params.FullCapNom / 16;

    EXPECT_CALL(i2c, I2CWriteMem(_, _, _, _)).WillRepeatedly(Return(II2C_OK));
    EXPECT_CALL(i2c, I2CReadMem(_, _, _, _)).WillRepeatedly(Return(II2C_OK));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(POR_STATUS_REG_VAL), Return(II2C_OK)))
        .WillRepeatedly(DoAll(SetArgNPointeeToUInt16<2>(DEFAULT_STATUS_REG_VAL), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_FSTAT_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(DATA_READY_FSTAT_REG_VALUE), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_MODELCFG_REG, _, _))
        .Times(1)
        .WillOnce(
            DoAll(SetArgNPointeeToUInt16<2>(REFRESH_DONE_MODELCFG_REG_VALUE), Return(II2C_OK)));

    /* Load from FS */
    EXPECT_CALL(fileSystem, fileSeek(_, _, fs::SeekFlag::ABSOLUTE_POS)).Times(1).WillOnce(Return(0));

    EXPECT_CALL(fileSystem, fileRead(_, _, sizeof(LearnedParameters)))
        .Times(1)
        .WillOnce(DoAll(SetArgPointerContent<1>(&params, sizeof(params)),
                        Return(sizeof(LearnedParameters))));

    /* Restore parameters */
    EXPECT_CALL(i2c, I2CWriteMem(_, MAX17055_RCOMP0_REG, _, _))
        .With(Args<2, 3>(ElementsAreArray({params.RCOMP0 & 0xff, params.RCOMP0 >> 8})))
        .Times(1);
    EXPECT_CALL(i2c, I2CWriteMem(_, MAX17055_TEMPCO_REG, _, _))
        .With(Args<2, 3>(ElementsAreArray({params.TempCo & 0xff, params.TempCo >> 8})))
        .Times(1);
    EXPECT_CALL(i2c, I2CWriteMem(_, MAX17055_FULLCAPNOM_REG, _, _))
        .With(Args<2, 3>(ElementsAreArray({params.FullCapNom & 0xff, params.FullCapNom >> 8})))
        .Times(1);
    EXPECT_CALL(i2c, I2CWriteMem(_, MAX17055_FULLCAPREP_REG, _, _))
        .With(Args<2, 3>(ElementsAreArray({params.FullCapRep & 0xff, params.FullCapRep >> 8})))
        .Times(1);

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_RCOMP0_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(params.RCOMP0), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_TEMPCO_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(params.TempCo), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_FULLCAPNOM_REG, _, _))
        .Times(2)
        .WillRepeatedly(DoAll(SetArgNPointeeToUInt16<2>(params.FullCapNom), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_MIXSOC_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(MIXSOC_REG_VALUE), Return(II2C_OK)));

    /* Write registers */
    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_MIXCAP_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(MIXCAP_REG_VALUE), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_FULLCAPREP_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(params.FullCapRep), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_DPACC_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(DPACC_REG_VALUE), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_DQACC_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(DQACC_REG_VALUE), Return(II2C_OK)));

    /* Last write */
    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_CYCLES_REG, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(params.Cycles), Return(II2C_OK)));


    EXPECT_TRUE(fuelGauge.configure(file));
}

TEST_F(MAX17055Test, InitI2CCommError)
{
    ON_CALL(i2c, I2CReadMem(_, _, _, _)).WillByDefault(Return(II2C_ERROR));

    EXPECT_FALSE(fuelGauge.init());
}

TEST_F(MAX17055Test, InitPORDataReadyTimeout)
{
    constexpr uint16_t POR_STATUS_REG_VAL = 1 << 1;
    constexpr uint16_t DEFAULT_STATUS_REG_VAL = 0;
    constexpr uint16_t DATA_NOT_READY_FSTAT_REG_VALUE = 1 << 0;

    EXPECT_CALL(i2c, I2CReadMem(_, _, _, _)).WillRepeatedly(Return(II2C_OK));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(POR_STATUS_REG_VAL), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_FSTAT_REG, _, _))
        .Times(AtLeast(2))
        .WillRepeatedly(
            DoAll(SetArgNPointeeToUInt16<2>(DATA_NOT_READY_FSTAT_REG_VALUE), Return(II2C_OK)));

    EXPECT_FALSE(fuelGauge.configure(file));
}

TEST_F(MAX17055Test, InitPORModelRefreshTimeout)
{
    constexpr uint16_t POR_STATUS_REG_VAL = 1 << 1;
    constexpr uint16_t DATA_READY_FSTAT_REG_VALUE = ~(1 << 0);
    constexpr uint16_t REFRESH_NOT_DONE_MODELCFG_REG_VALUE = 1 << 15;

    EXPECT_CALL(i2c, I2CReadMem(_, _, _, _)).WillRepeatedly(Return(II2C_OK));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(POR_STATUS_REG_VAL), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_FSTAT_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(DATA_READY_FSTAT_REG_VALUE), Return(II2C_OK)));

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_MODELCFG_REG, _, _))
        .Times(AtLeast(2))
        .WillRepeatedly(
            DoAll(SetArgNPointeeToUInt16<2>(REFRESH_NOT_DONE_MODELCFG_REG_VALUE), Return(II2C_OK)));

    EXPECT_FALSE(fuelGauge.configure(file));
}

TEST_F(MAX17055Test, BatteryInformation)
{
    initNoPORNominal();

    constexpr uint16_t BATTERY_CONNECTED_STATUS_REG_VALUE = 0;
    constexpr uint16_t REPSOC_REG_VALUE = 25600;
    constexpr uint16_t REPCAP_REG_VALUE = 3200;
    constexpr uint16_t TTF_REG_VALUE = 1000;
    constexpr uint16_t TTE_REG_VALUE = 2000;
    constexpr uint16_t CYCLES_REG_VALUE = 30;
    constexpr uint16_t AGE_REG_VALUE = 23040;

    ON_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillByDefault(
            DoAll(SetArgNPointeeToUInt16<2>(BATTERY_CONNECTED_STATUS_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_REPSOC_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(REPSOC_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_REPCAP_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(REPCAP_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_TTF_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(TTF_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_TTE_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(TTE_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_CYCLES_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(CYCLES_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_AGE_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(AGE_REG_VALUE), Return(II2C_OK)));

    BatteryInformation batteryInformation;
    EXPECT_TRUE(fuelGauge.getBatteryInformation(batteryInformation));
    EXPECT_EQ(batteryInformation.stateOfChargePercent, 100);
    EXPECT_EQ(batteryInformation.remainingCapacityMilliAh, 1600);
    EXPECT_EQ(batteryInformation.timeToFullSec, 5625);
    EXPECT_EQ(batteryInformation.timeToEmptySec, 11250);
    EXPECT_EQ(batteryInformation.cyclesHundredth, 30);
    EXPECT_EQ(batteryInformation.agePercent, 90);
};

TEST_F(MAX17055Test, BatteryInformationInfiniteTTF)
{
    initNoPORNominal();

    constexpr uint16_t BATTERY_CONNECTED_STATUS_REG_VALUE = 0;
    constexpr uint16_t TTF_REG_VALUE = UINT16_MAX;

    ON_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillByDefault(
            DoAll(SetArgNPointeeToUInt16<2>(BATTERY_CONNECTED_STATUS_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_TTF_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(TTF_REG_VALUE), Return(II2C_OK)));

    BatteryInformation batteryInformation;
    EXPECT_TRUE(fuelGauge.getBatteryInformation(batteryInformation));
    EXPECT_EQ(batteryInformation.timeToFullSec, UINT32_MAX);
};

TEST_F(MAX17055Test, BatteryInformationInfiniteTTE)
{
    initNoPORNominal();

    constexpr uint16_t BATTERY_CONNECTED_STATUS_REG_VALUE = 0;
    constexpr uint16_t TTE_REG_VALUE = UINT16_MAX;

    ON_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillByDefault(
            DoAll(SetArgNPointeeToUInt16<2>(BATTERY_CONNECTED_STATUS_REG_VALUE), Return(II2C_OK)));
    ON_CALL(i2c, I2CReadMem(_, MAX17055_TTE_REG, _, _))
        .WillByDefault(DoAll(SetArgNPointeeToUInt16<2>(TTE_REG_VALUE), Return(II2C_OK)));

    BatteryInformation batteryInformation;
    EXPECT_TRUE(fuelGauge.getBatteryInformation(batteryInformation));
    EXPECT_EQ(batteryInformation.timeToEmptySec, UINT32_MAX);
};

TEST_F(MAX17055Test, BatteryInformationNoBattery)
{
    initNoPORNominal();

    constexpr uint16_t NO_BATTERY_STATUS_REG_VALUE = 1 << 3;
    constexpr uint16_t DEFAULT_STATUS_REG_VALUE = 0;

    EXPECT_CALL(i2c, I2CReadMem(_, MAX17055_STATUS_REG, _, _))
        .WillOnce(DoAll(SetArgNPointeeToUInt16<2>(NO_BATTERY_STATUS_REG_VALUE), Return(II2C_OK)))
        .WillRepeatedly(DoAll(SetArgNPointeeToUInt16<2>(DEFAULT_STATUS_REG_VALUE), Return(II2C_OK)));

    BatteryInformation batteryInformation;
    EXPECT_FALSE(fuelGauge.getBatteryInformation(batteryInformation));
};

TEST_F(MAX17055Test, UpdateLearnedParametersNominal)
{
    initNoPORNominal();

    updateLearnedParameters(45, false);
    updateLearnedParameters(64, true);
    updateLearnedParameters(64, false);
    updateLearnedParameters(127, false);
    updateLearnedParameters(129, true);
};

TEST_F(MAX17055Test, BatteryInformationNoInit)
{
    BatteryInformation batteryInformation;
    EXPECT_FALSE(fuelGauge.getBatteryInformation(batteryInformation));
};

TEST_F(MAX17055Test, UpdateLearnedNoInit)
{
    EXPECT_FALSE(fuelGauge.updateLearnedParameters(file));
};
