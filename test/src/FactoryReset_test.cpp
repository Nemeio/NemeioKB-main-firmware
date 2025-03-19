#include "gtest/gtest.h"
#include "gtest_helpers.hpp"

#include "FactoryReset.hpp"
#include "MockApplication.hpp"
#include "MockSystem.hpp"
#include "MockBleChip.hpp"

using ::testing::Invoke;
using ::testing::Return;

class FactoryResetTest : public ::testing::Test
{
protected:
    FactoryResetTest()
        : factoryReset(application, system, bleChip){};

    MockApplication application;
    MockSystem system;
    MockBleChip bleChip;
    FactoryReset factoryReset;
};


TEST_F(FactoryResetTest, Nominal)
{
    auto bleFactoryResetResponse = [this](auto request)
    {
        factoryReset.notification(FactoryResetEvent{BLEDefs::ErrCode::SUCCESS});
        return BLEDefs::ErrCode::SUCCESS;
    };

    EXPECT_CALL(application, factoryReset).Times(1);
    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::FACTORY_RESET)))
        .Times(1)
        .WillOnce(Invoke(bleFactoryResetResponse));
    EXPECT_CALL(system, reset).Times(1);

    bool isActionBeforeResetCalled = false;
    factoryReset.run(
        [&isActionBeforeResetCalled](FactoryReset::ErrCode ret)
        {
            isActionBeforeResetCalled = true;
            EXPECT_EQ(ret, FactoryReset::ErrCode::SUCCESS);
        });

    EXPECT_TRUE(isActionBeforeResetCalled);
}

TEST_F(FactoryResetTest, BleTimeout)
{
    auto bleFactoryResetTimeout = [this](auto request)
    {
        factoryReset.notification(BleTxRequestTimeout{BLESPICommands::CommandId::FACTORY_RESET});
        return BLEDefs::ErrCode::SUCCESS;
    };

    EXPECT_CALL(application, factoryReset).Times(1);
    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::FACTORY_RESET)))
        .Times(1)
        .WillOnce(Invoke(bleFactoryResetTimeout));
    EXPECT_CALL(system, reset).Times(1);

    bool isActionBeforeResetCalled = false;
    factoryReset.run(
        [&isActionBeforeResetCalled](FactoryReset::ErrCode ret)
        {
            isActionBeforeResetCalled = true;
            EXPECT_EQ(ret, FactoryReset::ErrCode::BLE);
        });

    EXPECT_TRUE(isActionBeforeResetCalled);
}

TEST_F(FactoryResetTest, BleNotReady)
{
    EXPECT_CALL(application, factoryReset).Times(1);
    EXPECT_CALL(bleChip, sendRequest(IsBleCmdVariant(BLESPICommands::CommandId::FACTORY_RESET)))
        .Times(1)
        .WillOnce(Return(BLEDefs::ErrCode::DEVICE_NOT_READY));
    EXPECT_CALL(system, reset).Times(1);

    bool isActionBeforeResetCalled = false;
    factoryReset.run(
        [&isActionBeforeResetCalled](FactoryReset::ErrCode ret)
        {
            isActionBeforeResetCalled = true;
            EXPECT_EQ(ret, FactoryReset::ErrCode::BLE);
        });

    EXPECT_TRUE(isActionBeforeResetCalled);
}