#include "gtest/gtest.h"

#include "FactoryResetCmd.hpp"
#include "MockAppCommTxListener.hpp"
#include "MockFactoryReset.hpp"
#include "MockSystem.hpp"

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAreArray;
using ::testing::Invoke;
using ::testing::Return;

class FactoryResetCmdTest : public ::testing::Test
{
protected:
    FactoryResetCmdTest()
        : factoryResetCmd(txListener, factoryReset, system){};

    MockAppCommTxListener txListener;
    MockFactoryReset factoryReset;
    MockSystem system;
    FactoryResetCmd factoryResetCmd;

    static constexpr uint8_t successDataResp[] = {AppCommDefs::APPCOMMERR_SUCCESS};
    static constexpr uint8_t malformedDataResp[] = {AppCommDefs::APPCOMMERR_MALFORMED_DATA};
    static constexpr uint8_t bleErrorDataResp[] = {AppCommDefs::APPCOMMERR_BLE_CHIP_COMM_ERROR};
};

TEST_F(FactoryResetCmdTest, FactoryResetOk)
{
    auto CallActionBeforeReset = [](auto& actionBeforeReset)
    {
        if(actionBeforeReset)
        {
            actionBeforeReset(IFactoryReset::ErrCode::SUCCESS);
        }
    };

    EXPECT_CALL(factoryReset, run).Times(1).WillOnce(Invoke(CallActionBeforeReset));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::FACTORYRESET, _, _))
        .With(Args<1, 2>(ElementsAreArray(successDataResp)))
        .Times(1);
    EXPECT_CALL(system, delay).Times(1);

    factoryResetCmd.dataReceived(nullptr, 0);
}

TEST_F(FactoryResetCmdTest, FactoryResetWrongArgs)
{
    uint8_t wrongArgs[] = {0x00};
    EXPECT_CALL(factoryReset, run).Times(0);
    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::FACTORYRESET, _, _))
        .With(Args<1, 2>(ElementsAreArray(malformedDataResp)))
        .Times(1);
    factoryResetCmd.dataReceived(reinterpret_cast<const uint8_t*>(wrongArgs), sizeof(wrongArgs));
}

TEST_F(FactoryResetCmdTest, FactoryResetBLEError)
{
    auto CallActionBeforeReset = [](auto& actionBeforeReset)
    {
        if(actionBeforeReset)
        {
            actionBeforeReset(IFactoryReset::ErrCode::BLE);
        }
    };

    EXPECT_CALL(factoryReset, run).Times(1).WillOnce(Invoke(CallActionBeforeReset));

    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::FACTORYRESET, _, _))
        .With(Args<1, 2>(ElementsAreArray(bleErrorDataResp)))
        .Times(1);
    EXPECT_CALL(system, delay).Times(1);

    factoryResetCmd.dataReceived(nullptr, 0);
}