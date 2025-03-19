#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "KeepAliveCmd.hpp"
#include "MockAppCommTxListener.hpp"
#include "MockKeepAliveManager.hpp"
#include "MockKeyboardStatus.hpp"
#include "CborKeepAliveRespCodec.hpp"

using ::testing::_;
using ::testing::Args;
using ::testing::ElementsAreArray;
using ::testing::NiceMock;
using ::testing::Return;

class KeepAliveCmdTest : public ::testing::Test
{
protected:
    KeepAliveCmdTest()
        : cmd(txListener, keepAliveManager, keyboardStatus, codec)
    {
        ON_CALL(keyboardStatus, getLogSize()).WillByDefault(Return(48));
        ON_CALL(keyboardStatus, isErrorLogged()).WillByDefault(Return(true));
    };

    MockAppCommTxListener txListener;
    MockKeepAliveManager keepAliveManager;
    NiceMock<MockKeyboardStatus> keyboardStatus;
    CborKeepAliveRespCodec codec;
    KeepAliveCmd cmd;
};

TEST_F(KeepAliveCmdTest, Nominal)
{
    const uint8_t expectedResponse[] = {0xa1, 0x01, 0xa2, 0x01, 0x18, 0x30, 0x02, 0xf5};

    EXPECT_CALL(keyboardStatus, refresh()).Times(1);
    EXPECT_CALL(txListener,
                sendResponse(AppCommDefs::AppCommCmd::KEEPALIVE, _, sizeof(expectedResponse)))
        .With(Args<1, 2>(ElementsAreArray(expectedResponse)))
        .Times(1);
    EXPECT_CALL(keepAliveManager, receivedKeepAlive()).Times(1);

    uint8_t data = 0;
    uint8_t* pData = &data;
    size_t dataLen = 0;
    cmd.dataReceived(pData, dataLen);
};

TEST_F(KeepAliveCmdTest, NominalNull)
{
    EXPECT_CALL(keyboardStatus, refresh()).Times(1);
    EXPECT_CALL(txListener, sendResponse(AppCommDefs::AppCommCmd::KEEPALIVE, _, _)).Times(1);
    EXPECT_CALL(keepAliveManager, receivedKeepAlive()).Times(1);

    uint8_t* pData = nullptr;
    size_t dataLen = 0;
    cmd.dataReceived(pData, dataLen);
};

TEST_F(KeepAliveCmdTest, WithData)
{
    EXPECT_CALL(txListener, sendResponse).Times(0);
    EXPECT_CALL(keepAliveManager, receivedKeepAlive()).Times(0);

    uint8_t data = 0;
    uint8_t* pData = &data;
    size_t dataLen = sizeof(data);
    cmd.dataReceived(pData, dataLen);
};

TEST_F(KeepAliveCmdTest, WithDataNull)
{
    EXPECT_CALL(txListener, sendResponse).Times(0);
    EXPECT_CALL(keepAliveManager, receivedKeepAlive()).Times(0);

    uint8_t* pData = nullptr;
    size_t dataLen = 1;
    cmd.dataReceived(pData, dataLen);
};