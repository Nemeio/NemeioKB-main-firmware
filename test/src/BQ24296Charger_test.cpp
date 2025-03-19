#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "gtest_helpers.hpp"
#include "BQ24296Charger.h"
#include "MockI2C.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Pointee;
using ::testing::Return;

class BQ24296ChargerTest : public ::testing::Test
{
protected:
    BQ24296ChargerTest()
        : charger(i2c){};

    NiceMock<MockI2C> i2c;
    BQ24296Charger charger;
};

TEST_F(BQ24296ChargerTest, InitNominal)
{
    uint8_t expectedByte = 0x10;
    uint8_t vendorId = 0x01 << 5;
    uint8_t expectedByteReg5 = 0xCF;
    uint8_t Reg5init = 0xFF;

    EXPECT_CALL(i2c, I2CReadMem(BQ24296_I2C_ADDRESS, _, _, _)).WillRepeatedly(Return(II2C_OK));

    EXPECT_CALL(i2c, I2CReadMem(BQ24296_I2C_ADDRESS, VENDOR_REG, _, sizeof(vendorId)))
        .Times(1)
        .WillOnce(DoAll(SetArgPointerContent<2>(&vendorId, sizeof(vendorId)), Return(II2C_OK)));

    EXPECT_CALL(i2c,
                I2CWriteMem(BQ24296_I2C_ADDRESS,
                            PRECHARGE_TERMINATION_CURRENT_CTRL_REG,
                            Pointee(Eq(expectedByte)),
                            sizeof(expectedByte)))
        .Times(1);


    EXPECT_CALL(i2c,
                I2CReadMem(BQ24296_I2C_ADDRESS,
                           PRECHARGE_TERMINATION_VOLTAGE_CTRL_REG,
                           _,
                           sizeof(Reg5init)))
        .Times(2)
        .WillOnce(DoAll(SetArgPointerContent<2>(&Reg5init, sizeof(Reg5init)), Return(II2C_OK)))
        .WillOnce(Return(II2C_OK));

    EXPECT_CALL(i2c,
                I2CWriteMem(BQ24296_I2C_ADDRESS,
                            PRECHARGE_TERMINATION_VOLTAGE_CTRL_REG,
                            Pointee(Eq(expectedByteReg5)),
                            sizeof(expectedByteReg5)))
        .Times(1);

    charger.init();
}
