#include "TraceLogger.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class TraceLoggerTest : public ::testing::Test
{
protected:
    TraceLoggerTest()
    {
        logger::TraceLogger::instance().clearError();
        logger::TraceLogger::instance().oss().clear();
    }
};

TEST_F(TraceLoggerTest, LogInfo)
{
    std::string expectedString("<INF> test: This is a test\r\n");
    etl::string<50> str;

    LOG_INFO("test", "This is a test");

    logger::TraceLogger::instance().oss().to_string(str);
    EXPECT_EQ(str.c_str(), expectedString);
    EXPECT_FALSE(logger::TraceLogger::instance().isErrorLogged());
}

TEST_F(TraceLoggerTest, LogWarning)
{
    logger::TraceLogger::instance().oss().clear();
    std::string expectedString("<WRN> test: This is a test\r\n");
    etl::string<50> str;

    LOG_WARN("test", "This is a test");

    logger::TraceLogger::instance().oss().to_string(str);
    EXPECT_EQ(str.c_str(), expectedString);
    EXPECT_FALSE(logger::TraceLogger::instance().isErrorLogged());
}

TEST_F(TraceLoggerTest, LogError)
{
    logger::TraceLogger::instance().oss().clear();
    std::string expectedString("<ERR> test: This is a test\r\n");
    etl::string<50> str;

    LOG_ERR("test", "This is a test");

    logger::TraceLogger::instance().oss().to_string(str);
    EXPECT_EQ(str.c_str(), expectedString);
    EXPECT_TRUE(logger::TraceLogger::instance().isErrorLogged());
}

TEST_F(TraceLoggerTest, LogErrorStatus)
{
    logger::TraceLogger::instance().clearError();
    EXPECT_FALSE(logger::TraceLogger::instance().isErrorLogged());
    logger::TraceLogger::instance().setError();
    EXPECT_TRUE(logger::TraceLogger::instance().isErrorLogged());
    logger::TraceLogger::instance().clearError();
    EXPECT_FALSE(logger::TraceLogger::instance().isErrorLogged());
}