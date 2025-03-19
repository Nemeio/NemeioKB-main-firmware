#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ReceiveTraceLogCmd.hpp"
#include "MockTraceLogger.hpp"

using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

class ReceiveTraceLogCmdTest : public ::testing::Test
{
protected:
    ReceiveTraceLogCmdTest()
        : cmd(logger)
    {
        ON_CALL(logger, oss).WillByDefault(ReturnRef(stream));
        ON_CALL(logger, lock).WillByDefault(Return(true));
        ON_CALL(logger, unlock).WillByDefault(Return(true));
    };

    logger::TraceLoggerStream stream;
    NiceMock<MockTraceLogger> logger;
    ReceiveTraceLogCmd cmd;
};

TEST_F(ReceiveTraceLogCmdTest, Nominal)
{
    std::string str("This is a test");
    stream << str.c_str();

    size_t totalSize = 0;
    size_t expectedSize = stream.size();

    EXPECT_CALL(logger, clearError).Times(1);

    EXPECT_EQ(cmd.prepareReceive(nullptr, 0, totalSize), AppCommDefs::APPCOMMERR_SUCCESS);
    EXPECT_EQ(totalSize, expectedSize);

    Mock::VerifyAndClearExpectations(&logger);

    std::array<uint8_t, 50> data;
    size_t receiveSize = 0;

    EXPECT_EQ(cmd.receiveData(data.data(), data.size(), receiveSize),
              AppCommDefs::APPCOMMERR_SUCCESS);
    EXPECT_EQ(receiveSize, str.size());
    EXPECT_EQ(std::string(data.data(), data.data() + receiveSize), str);
    EXPECT_EQ(stream.size(), 0);

    EXPECT_EQ(cmd.terminateReceive(), AppCommDefs::APPCOMMERR_SUCCESS);
}

TEST_F(ReceiveTraceLogCmdTest, ReceiveToSmallerBuffer)
{
    std::string str("This is a test");
    stream << str.c_str();

    size_t totalSize = 0;
    EXPECT_CALL(logger, clearError).Times(1);
    cmd.prepareReceive(nullptr, 0, totalSize);

    std::array<uint8_t, 10> data;
    size_t receiveSize = 0;

    std::string expectedStr("This is a ");
    EXPECT_EQ(cmd.receiveData(data.data(), data.size(), receiveSize),
              AppCommDefs::APPCOMMERR_SUCCESS);
    EXPECT_EQ(receiveSize, expectedStr.size());
    EXPECT_EQ(std::string(data.data(), data.data() + receiveSize), expectedStr);

    std::string expectedStr2("test");
    EXPECT_EQ(cmd.receiveData(data.data(), data.size(), receiveSize),
              AppCommDefs::APPCOMMERR_SUCCESS);
    EXPECT_EQ(receiveSize, expectedStr2.size());
    EXPECT_EQ(std::string(data.data(), data.data() + receiveSize), expectedStr2);
}