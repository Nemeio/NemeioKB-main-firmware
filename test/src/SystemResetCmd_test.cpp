#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SystemResetCmd.hpp"
#include "mocks/MockAppCommTxListener.hpp"
#include "mocks/MockSystem.hpp"

using testing::_;
using testing::Args;
using testing::ElementsAreArray;

class SystemResetCmdTest : public ::testing::Test
{
protected:
    SystemResetCmdTest()
        : cmd(txListener, system){};

    MockAppCommTxListener txListener;
    MockSystem system;
    SystemResetCmd cmd;

    static constexpr uint8_t errDataResp[] = {AppCommDefs::APPCOMMERR_MALFORMED_DATA};
};

TEST_F(SystemResetCmdTest, Nominal)
{
    EXPECT_CALL(txListener, sendResponse).Times(0);
    EXPECT_CALL(system, reset()).Times(1);

    uint8_t data = 0;
    uint8_t* pData = &data;
    size_t dataLen = 0;
    cmd.dataReceived(pData, dataLen);
};

TEST_F(SystemResetCmdTest, NominalNull)
{
    EXPECT_CALL(txListener, sendResponse).Times(0);
    EXPECT_CALL(system, reset()).Times(1);

    uint8_t* pData = nullptr;
    size_t dataLen = 0;
    cmd.dataReceived(pData, dataLen);
};

TEST_F(SystemResetCmdTest, WithData)
{
    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::SYSTEMRESET, _, _))
        .With(Args<1, 2>(ElementsAreArray(errDataResp)))
        .Times(1);
    EXPECT_CALL(system, reset()).Times(0);

    uint8_t data = 0;
    uint8_t* pData = &data;
    size_t dataLen = sizeof(data);
    cmd.dataReceived(pData, dataLen);
};

TEST_F(SystemResetCmdTest, WithDataNull)
{
    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::SYSTEMRESET, _, _))
        .With(Args<1, 2>(ElementsAreArray(errDataResp)))
        .Times(1);
    EXPECT_CALL(system, reset()).Times(0);

    uint8_t* pData = nullptr;
    size_t dataLen = 1;
    cmd.dataReceived(pData, dataLen);
};